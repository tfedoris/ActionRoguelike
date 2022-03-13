// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpBase.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickUpBase
{
	GENERATED_BODY()

public:
	ASHealthPotion();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing Properties")
	float HealingValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Properties")
	float HiddenDuration;
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
