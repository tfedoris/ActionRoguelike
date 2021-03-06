// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"

#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MatineeCameraShake.h"
#include "SGameplayFunctionLibrary.h"
#include "ActionRoguelike/ActionRoguelike.h"

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

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;

	bReplicates = true;
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

void ASProjectile::ProjectileHit(AActor* OtherActor, FVector HitLocation)
{
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	HandleImpactEffects(OtherActor, HitLocation);
	
	Destroy();
}

void ASProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileHit(OtherActor, Hit.ImpactPoint);
}

void ASProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Implement in derived classes
}

void ASProjectile::ChangeInstigator(AActor* NewInstigator)
{
	SphereComp->ClearMoveIgnoreActors();
	SetInstigator(Cast<APawn>(NewInstigator));
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASProjectile::HandleImpactEffects(AActor* OtherActor, FVector HitLocation)
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactShake, HitLocation, ImpactShakeInnerRadius, ImpactShakeOuterRadius);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitLocation, GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundCue, HitLocation);
}

// Called every frame
void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

