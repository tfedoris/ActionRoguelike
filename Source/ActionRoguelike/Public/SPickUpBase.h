// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickUpBase.generated.h"

class USphereComponent;
class UAudioComponent;
class UParticleSystem;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPickUpBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpBase();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	virtual bool CanInteract_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Properties")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Properties")
	float bCanRespawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interact")
	bool bCanInteract;

	UPROPERTY(ReplicatedUsing="OnRep_IsActive", BlueprintReadOnly)
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* PickUpEffect;

	FTimerHandle TimerHandle_HiddenDuration;

	virtual void HandlePickUp(AActor* OtherActor);

	UFUNCTION(Server, Reliable)
	virtual void ServerHandlePickup(AActor* OtherActor);

	UFUNCTION(Client, Unreliable)
	virtual void ClientHandlePickupEffects();
	
	virtual void OnHiddenDurationElapsed();

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnRep_IsActive();

	UFUNCTION()
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
