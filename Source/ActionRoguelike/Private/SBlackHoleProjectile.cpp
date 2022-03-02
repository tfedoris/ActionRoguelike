// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackHoleProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASBlackHoleProjectile::ASBlackHoleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 1000.0f;
	RadialForceComp->ForceStrength = -2000000.0f;
	RadialForceComp->SetupAttachment(RootComponent);
	
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ASBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBlackHoleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

