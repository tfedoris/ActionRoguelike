// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	DamagePercent = 0.1;
	Duration = 0.0f;
	Period = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	USAttributeComponent* OwningAttributeComponent = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (OwningAttributeComponent)
	{
		OwningAttributeComponent->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	USAttributeComponent* OwningAttributeComponent = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (OwningAttributeComponent)
	{
		OwningAttributeComponent->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float ActualDelta, float Delta)
{
	if (ActualDelta >= 0.0f || InstigatorActor == GetOwningComponent()->GetOwner())
	{
		return;
	}

	USAttributeComponent* InstigatorAttributeComp = USAttributeComponent::GetAttributes(InstigatorActor);
	if (InstigatorAttributeComp)
	{
		InstigatorAttributeComp->ApplyHealthChange(InstigatorActor, FMath::RoundToFloat(ActualDelta * DamagePercent));
	}
}
