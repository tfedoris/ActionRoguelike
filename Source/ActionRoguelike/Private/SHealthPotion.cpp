// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

ASHealthPotion::ASHealthPotion()
{
	HealingValue = 50.0f;
	HiddenDuration = 10.0f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	HealActor(InstigatorPawn);
}

void ASHealthPotion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASHealthPotion::OnActorBeginOverlap);
}

void ASHealthPotion::HealActor(AActor* ActorToHeal)
{
	if (ActorToHeal)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(ActorToHeal->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->GetCurrentHealth() < AttributeComp->GetMaxHealth())
		{
			PlayPickUpEffect();
			AttributeComp->ApplyHealthChange(this, HealingValue);
			SetActorHiddenInGame(true);
			GetWorldTimerManager().SetTimer(TimerHandle_HiddenDuration, this, &ASHealthPotion::OnHiddenDurationElapsed, HiddenDuration);
		}
	}
}

void ASHealthPotion::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// HealActor(OtherActor);
}
