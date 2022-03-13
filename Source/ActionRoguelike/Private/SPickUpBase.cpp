// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASPickUpBase::ASPickUpBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetAutoActivate(false);
	AudioComp->SetupAttachment(RootComponent);
}

void ASPickUpBase::PlayPickUpEffect() const
{
	if (PickUpEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickUpEffect, GetActorLocation());
	}

	AudioComp->Play();
}

void ASPickUpBase::OnHiddenDurationElapsed()
{
	SetActorHiddenInGame(false);
}
