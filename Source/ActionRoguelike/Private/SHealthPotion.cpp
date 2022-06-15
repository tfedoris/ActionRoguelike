// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"
#include "Components/SphereComponent.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASHealthPotion::ASHealthPotion()
{
	HealingValue = 50.0f;
	CreditsCost = 100;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	HealActor(InstigatorPawn);
}

FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Costs {0} Credits. Restores health to maximum"), CreditsCost);
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
		
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(ActorToHeal);
		if (AttributeComp && AttributeComp->GetHealth() < AttributeComp->GetMaxHealth())
		{
			HandlePickUp(ActorToHeal);
			PlayerState->RemoveCredits(CreditsCost);
			AttributeComp->ApplyHealthChange(this, HealingValue);
		}
	}
}

void ASHealthPotion::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

#undef LOCTEXT_NAMESPACE
