// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;
class ASPlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCreditsTotal, int32, Delta);

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
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_TotalCredits", Category = "Credits")
	int32 TotalCredits;

	UFUNCTION(Server, Reliable)
	void ServerSetCredits(int32 NewTotalCredits);

	// // Downside of using multicast here is that we send over more data over the net, since it's an RPS with two parameters. OnRep_ is "free" since Credits is already getting replicated anyway.
	// UFUNCTION(NetMulticast, Unreliable)
	// void MulticastCreditsChanged(int32 NewCreditsTotal, int32 Delta);

	//OnRep_ functions can use a parameter containing the 'old value' of the variable it is bound to. Very useful in this case to figure out the 'delta'.
	UFUNCTION()
	void OnRep_TotalCredits(int32 OldTotalCredits);
};
