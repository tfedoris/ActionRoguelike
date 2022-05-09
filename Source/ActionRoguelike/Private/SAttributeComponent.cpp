// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.0f;
	MaxRage = 100.0f;
	Rage = 0.0f;
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (!FromActor)
	{
		return nullptr;
	}

	return Cast<USAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) // God-Mode is enabled
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	const float PrevHealth = Health;

	Health = FMath::Clamp(Health += Delta, 0.0f, MaxHealth);

	const float ActualDelta = Health - PrevHealth;

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta, Delta);
	}

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (Rage + Delta < 0.0f )
	{
		return false;
	}

	float PrevRage = Rage;
	
	Rage = FMath::Clamp(Rage += Delta, 0.0f, MaxRage);

	float ActualDelta = Rage - PrevRage;
	OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta, Delta);

	return true;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
	float ActualDelta, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, ActualDelta, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
	// DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_InitialOnly);
}
