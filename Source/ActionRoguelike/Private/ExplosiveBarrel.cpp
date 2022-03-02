// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "SMagicProjectile.h"
#include "ActionRoguelike/ActionRoguelikeGameModeBase.h"
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

	AActionRoguelikeGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AActionRoguelikeGameModeBase>();
	if (GameMode->bShowDebugLines)
	{
		UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

		// %s = string
		// %f = float
		// logs: "OtherActor: MyActor_1, at game time: 124.4"
		UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.f, true);
	}
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

