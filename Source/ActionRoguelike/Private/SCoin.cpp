// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"

#include "SPlayerState.h"
#include "Components/SphereComponent.h"

void ASCoin::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	CreditValue = 50;
}

void ASCoin::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	ASPlayerState* PlayerState = ASPlayerState::GetPlayerState(OtherActor);
	if (!PlayerState)
	{
		return;
	}
	
	PlayerState->AddCredits(CreditValue);

	Super::OnActorBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
