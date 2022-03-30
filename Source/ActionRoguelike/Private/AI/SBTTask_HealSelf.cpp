// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"

USBTTask_HealSelf::USBTTask_HealSelf()
{
	HealPercent = 1.0f;
}

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!ensure(Controller))
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = Controller->GetPawn();
	if (!ensure(AIPawn))
	{
		return EBTNodeResult::Failed;
	}
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);
	if (!ensureMsgf(AttributeComp, TEXT("Attribute Comp was nullptr! Please assign an Attribute Component to the AI Character.")))
	{
		return EBTNodeResult::Failed;
	}

	const bool bHealSuccess = AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetMaxHealth() * HealPercent);

	return bHealSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
