// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "SMagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName("PhysicsBody");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->Radius = 600.0f;
	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->SetupAttachment(RootComponent);

	ExplosionEffectScale = 1.0f;
	ExplosionEffectScaleVector = FVector(1.0f, 1.0f, 1.0f);
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}

void AExplosiveBarrel::Explode()
{
	if (ExplodedMaterial)
	{
		MeshComp->SetMaterial(0, ExplodedMaterial);
	}
	
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), ExplosionEffectScaleVector * ExplosionEffectScale);
	}
	
	RadialForceComp->FireImpulse();
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

