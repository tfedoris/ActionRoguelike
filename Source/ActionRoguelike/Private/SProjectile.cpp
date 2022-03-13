// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"

#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MatineeCameraShake.h"

// Sets default values
ASProjectile::ASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->InitSphereRadius(16.0f);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 8000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	BaseDamage = 20.0f;

	ImpactShakeInnerRadius = 100.0f;
	ImpactShakeOuterRadius = 2000.0f;
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectile::OnActorHit);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Called when the game starts or when spawned
void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASProjectile::ProjectileHit(FVector HitLocation)
{
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactShake, HitLocation, ImpactShakeInnerRadius, ImpactShakeOuterRadius);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitLocation, GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundCue, HitLocation);
	
	Destroy();
}

void ASProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileHit(Hit.ImpactPoint);
}

void ASProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			ProjectileHit(GetActorLocation());
			AttributeComp->ApplyHealthChange(-BaseDamage);
		}
	}
}

// Called every frame
void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

