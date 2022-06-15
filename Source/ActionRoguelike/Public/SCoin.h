// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpBase.h"
#include "SCoin.generated.h"



/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASPickUpBase
{
	GENERATED_BODY()

public:
	ASCoin();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	int32 CreditValue;
	
	virtual void PostInitializeComponents() override;
	
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(Server, Reliable)
	void ServerOnActorBeginOverlap(AActor* OtherActor);
};
