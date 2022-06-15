// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, ActualDelta, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const
	{
		return Health;
	}

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const
	{
		return MaxHealth;
	}

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRage() const
	{
		return Rage;
	}

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxRage() const
	{
		return MaxRage;
	}

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float IsFullHealth() const
	{
		return Health == MaxHealth;
	}

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxRage;
	
	// UPROPERTY(ReplicatedUsing="")
	// bool bIsAlive;
	
	UFUNCTION(NetMulticast, Reliable) // TODO: Mark as unreliable once we have moved the 'state' out of SCharacter (consider using the above commented-out code as a starting point)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float ActualDelta, float Delta);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float ActualDelta, float Delta);

public:
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);
};
