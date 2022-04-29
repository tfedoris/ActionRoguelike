// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "SPickUpBase.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickUpBase, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float HealingValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	int32 CreditsCost;

	void HealActor(AActor* ActorToHeal);

	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
