// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUp.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPickUp::ASPickUp()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
}

