// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class AActionRoguelikeGameModeBase;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpecialProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SpecialAttack;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY()
	AActionRoguelikeGameModeBase* GameMode;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	FName ProjectileAttackSocketName;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	float AimRange;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void SpecialAttack();
	void PrimaryInteract();

	UFUNCTION()
	void ProjectileAttack(TSubclassOf<AActor> ProjectileClass);
	
	void VisualizePlayerRotation() const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
