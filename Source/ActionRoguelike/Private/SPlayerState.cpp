// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	TotalCredits = 0;
}

ASPlayerState* ASPlayerState::GetPlayerState(AActor* FromActor)
{
	if (!IsValid(FromActor) || !IsValid(FromActor->GetInstigatorController()))
	{
		return nullptr;
	}
	
	return Cast<ASPlayerState>(FromActor->GetInstigatorController()->PlayerState);
}

void ASPlayerState::AddCredits(const int32 Credits)
{
	if (HasAuthority())
	{
		int32 OldTotalCredits = TotalCredits;
		
		TotalCredits = TotalCredits + Credits;

		OnRep_TotalCredits(OldTotalCredits);
	}
}

void ASPlayerState::RemoveCredits(const int32 Credits)
{
	if (HasAuthority())
	{
		int32 OldTotalCredits = TotalCredits;
		
		TotalCredits = TotalCredits - Credits;
		TotalCredits = FMath::Max(0, TotalCredits);
		
		OnRep_TotalCredits(OldTotalCredits);
	}
}

void ASPlayerState::OnRep_TotalCredits(int32 OldTotalCredits)
{
	ForceNetUpdate();
	OnCreditsChanged.Broadcast(this, TotalCredits, TotalCredits - OldTotalCredits);
}

void ASPlayerState::ServerSetCredits_Implementation(int32 NewTotalCredits)
{
	TotalCredits = NewTotalCredits;
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		TotalCredits = SaveObject->Credits;
	}
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = TotalCredits;
	}
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, TotalCredits);
}
