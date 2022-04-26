// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Trigger when health is changed (damage/healing)
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float ActualDelta, float Delta)
{
	if (ActualDelta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

