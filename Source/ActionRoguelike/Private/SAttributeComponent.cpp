// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float PrevHealth = Health;

	Health = FMath::Clamp(Health += Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - PrevHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

	return ActualDelta != 0;
}
