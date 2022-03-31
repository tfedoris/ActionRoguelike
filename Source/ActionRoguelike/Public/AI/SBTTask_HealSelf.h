// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_HealSelf.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTask_HealSelf : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USBTTask_HealSelf();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
	
	struct FBTHealSelfMemory
	{
		float TimeElapsed = 0.f;
		float StartingHealth = 0.f;
		bool bHasStartedHealing = false;
	};
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI", meta=(ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float HealPercent;

	UPROPERTY(EditAnywhere, Category = "AI", meta=(ClampMin = "0.0", UIMin = "0.0"))
	float HealDuration;
};
