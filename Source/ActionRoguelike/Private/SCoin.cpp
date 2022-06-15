// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"

#include "SPlayerState.h"
#include "Components/SphereComponent.h"

ASCoin::ASCoin()
{
	CreditValue = 50;
	bCanInteract = false;
}

void ASCoin::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ASCoin::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ServerOnActorBeginOverlap(OtherActor);

	Super::OnActorBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ASCoin::ServerOnActorBeginOverlap_Implementation(AActor* OtherActor)
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
}
