// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SActionEffect.h"
#include "SActionComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::HandleImpactEffects(AActor* OtherActor, FVector HitLocation)
{
	Super::HandleImpactEffects(OtherActor, HitLocation);

	if (!OtherActor)
	{
		return;
	}
	
	USActionComponent* ActionComp = USActionComponent::GetActionComponent(OtherActor);
	if (BurningActionClass && ActionComp && HasAuthority())
	{
		ActionComp->AddAction(GetInstigator(), BurningActionClass);
	}
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

