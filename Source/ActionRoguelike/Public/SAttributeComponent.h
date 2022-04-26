// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, ActualDelta, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChanged, USAttributeComponent*, OwningComp, float, NewRage);

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

	float GetCurrentHealth() const
	{
		return Health;
	}

	float GetMaxHealth() const
	{
		return MaxHealth;
	}

	float GetCurrentRage() const
	{
		return Rage;
	}

	float GetMaxRage() const
	{
		return MaxRage;
	}

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxRage;

	// TODO: Stamina, Strength

public:
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(float Delta);
};
