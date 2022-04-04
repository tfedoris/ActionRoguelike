// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, float, NewCreditsTotal, float, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category = "State")
	static ASPlayerState* GetPlayerState(AActor* FromActor);

	UFUNCTION(BlueprintCallable)
	int GetTotalCredits() const
	{
		return TotalCredits;
	}

	void AddCredits(int Credits);
	void RemoveCredits(int Credits);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

protected:
	int TotalCredits;
};
