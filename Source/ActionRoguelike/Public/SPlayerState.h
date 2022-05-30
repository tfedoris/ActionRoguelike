// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, int32, NewCreditsTotal, int32, Delta);

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
	int32 GetTotalCredits() const
	{
		return TotalCredits;
	}

	void AddCredits(int32 Credits);
	void RemoveCredits(int32 Credits);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

protected:
	int32 TotalCredits;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCreditsChanged(int32 NewCreditsTotal, int32 Delta);
};
