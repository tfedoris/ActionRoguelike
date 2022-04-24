// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SParryableProjectile.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASParryableProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect> BurningActionClass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleImpactEffects(AActor* OtherActor, FVector HitLocation) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
