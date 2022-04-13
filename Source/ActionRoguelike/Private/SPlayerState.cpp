// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

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

void ASPlayerState::AddCredits(const int Credits)
{
	TotalCredits += Credits;
	OnCreditsChanged.Broadcast(TotalCredits, Credits);
}

void ASPlayerState::RemoveCredits(const int Credits)
{
	TotalCredits -= Credits;
	TotalCredits = FMath::Max(0, TotalCredits);
	OnCreditsChanged.Broadcast(TotalCredits, -Credits);
}
