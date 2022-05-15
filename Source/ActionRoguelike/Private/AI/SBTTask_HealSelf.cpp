// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"

USBTTask_HealSelf::USBTTask_HealSelf()
{
	bNotifyTick = true;
	HealPercent = 1.0f;
	HealDuration = 2.0f;
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

	FBTHealSelfMemory* MyMemory = reinterpret_cast<FBTHealSelfMemory*>(NodeMemory);
	MyMemory->bHasStartedHealing = true;
	MyMemory->TimeElapsed = 0.f;
	MyMemory->StartingHealth = AttributeComp->GetHealth();

	return EBTNodeResult::InProgress;
}

void USBTTask_HealSelf::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!ensure(Controller))
	{
		return;
	}

	APawn* AIPawn = Controller->GetPawn();
	if (!ensure(AIPawn))
	{
		return;
	}
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);
	if (!ensureMsgf(AttributeComp, TEXT("Attribute Comp was nullptr! Please assign an Attribute Component to the AI Character.")))
	{
		return;
	}

	FBTHealSelfMemory* MyMemory = reinterpret_cast<FBTHealSelfMemory*>(NodeMemory);

	MyMemory->TimeElapsed += DeltaSeconds;
	const float Alpha = MyMemory->TimeElapsed / HealDuration;
	const float TargetHealth = FMath::Lerp(MyMemory->StartingHealth, AttributeComp->GetMaxHealth() * HealPercent, Alpha);
	const float CurrentHealth = AttributeComp->GetHealth();
	const float Delta = TargetHealth - CurrentHealth;
	
	AttributeComp->ApplyHealthChange(AIPawn, Delta);

	if (TargetHealth >= AttributeComp->GetMaxHealth() * HealPercent)
	{
		MyMemory->TimeElapsed = 0.f;
		MyMemory->StartingHealth = 0.f;
		MyMemory->bHasStartedHealing = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 USBTTask_HealSelf::GetInstanceMemorySize() const
{
	return sizeof(FBTHealSelfMemory);
}
