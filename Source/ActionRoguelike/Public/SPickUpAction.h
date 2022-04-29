// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpBase.h"
#include "SPickUpAction.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPickUpAction : public ASPickUpBase
{
	GENERATED_BODY()

public:
	ASPickUpAction();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	TSubclassOf<USAction> ActionClass;
	
	virtual void HandlePickUp(AActor* OtherActor) override;
};
