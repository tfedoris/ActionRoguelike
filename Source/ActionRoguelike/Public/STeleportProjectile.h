// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	ASTeleportProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float ProjectileDuration;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* PortalOpenEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* PortalExitEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* PortalCloseEffect;
	
	FTimerHandle TimerHandle_ProjectileDuration;
	FTimerHandle TimerHandle_TeleportDelay;

	virtual void BeginPlay() override;
	virtual void ProjectileHit(FVector HitLocation) override;
	void OnProjectileDurationElapsed();
	void OnTeleportDelayEnd();
	void TeleportInstigator();
};
