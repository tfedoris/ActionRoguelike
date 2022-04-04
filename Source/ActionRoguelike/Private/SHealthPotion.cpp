// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"
#include "Components/SphereComponent.h"

ASHealthPotion::ASHealthPotion()
{
	HealingValue = 50.0f;
	CreditsCost = 100.0f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	HealActor(InstigatorPawn);
}

void ASHealthPotion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASHealthPotion::OnActorBeginOverlap);
}

void ASHealthPotion::HealActor(AActor* ActorToHeal)
{
	if (ActorToHeal)
	{
		ASPlayerState* PlayerState = ASPlayerState::GetPlayerState(ActorToHeal);
		if (!PlayerState)
		{
			return;
		}
		if (PlayerState->GetTotalCredits() < CreditsCost)
		{
			
			return;
		}
		
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(ActorToHeal->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->GetCurrentHealth() < AttributeComp->GetMaxHealth())
		{
			HandlePickUp();
			PlayerState->RemoveCredits(CreditsCost);
			AttributeComp->ApplyHealthChange(this, HealingValue);
		}
	}
}

void ASHealthPotion::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// HealActor(OtherActor);
}
