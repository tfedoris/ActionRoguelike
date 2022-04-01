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

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attacks & Abilities")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attacks & Abilities")
	TSubclassOf<AActor> SpecialProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attacks & Abilities")
	TSubclassOf<AActor> MovementProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attacks & Abilities")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	FVector AimStart;
	FVector AimEnd;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SpecialAttack;
	FTimerHandle TimerHandle_MovementAbility;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* CastParticleSystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Socket Names")
	FName ProjectileAttackSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashColorParamName;

	float AimRange;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void GetAimStartAndEnd(FVector& Start, FVector& End);
	void PrimaryAttack();
	void SpecialAttack();
	void PrimaryInteract();
	void MovementAbility();

	UFUNCTION()
	void ProjectileAttack(TSubclassOf<AActor> ProjectileClass);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

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
