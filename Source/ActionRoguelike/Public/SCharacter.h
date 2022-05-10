// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class ASGameModeBase;
class USAttributeComponent;
class UParticleSystemComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashColorParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actions")
	FName PrimaryAttackActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actions")
	FName SpecialAttackActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actions")
	FName MovementAbilityActionName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
	float RageMultiplier;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void SprintStart();

	void SprintStop();
	
	void PrimaryAttack();
	
	void SpecialAttack();

	void MovementAbility();

	void PrimaryInteract();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float ActualDelta, float Delta);

	virtual FVector GetPawnViewLocation() const override;
	
	void VisualizePlayerRotation() const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
};
