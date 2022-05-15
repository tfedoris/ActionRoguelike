// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
	LowHealthThresholdPercent = 0.3f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(!ensure(BlackboardComp))
	{
		return;
	}

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

	float CurrentHealth = AttributeComp->GetHealth();
	float MaxHealth = AttributeComp->GetMaxHealth();
	BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, (CurrentHealth <= MaxHealth * LowHealthThresholdPercent));
}
