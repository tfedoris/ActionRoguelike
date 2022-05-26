// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickUpBase.generated.h"

class USphereComponent;
class UAudioComponent;
class UParticleSystem;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpBase(); //  poke someone tonight and get a boner 

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

	UPROPERTY(ReplicatedUsing="OnRep_CollisionChanged", BlueprintReadOnly)
	bool bEnableCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* PickUpEffect;

	FTimerHandle TimerHandle_HiddenDuration;

	virtual void HandlePickUp(AActor* OtherActor);

	virtual void OnHiddenDurationElapsed();

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnRep_CollisionChanged();

	UFUNCTION()
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
