// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:
	USAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	FName CastSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AimRange;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float RageCost;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	void GetTraceStartAndEnd(AController* Controller, FVector& Start, FVector& End);
};
