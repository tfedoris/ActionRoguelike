// Fill out your copyright notice in the Description page of Project Settings.


#include "SParryableProjectile.h"

#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void ASParryableProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USActionComponent* ActionComp = USActionComponent::GetActionComponent(OtherActor);
	if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
	{
		MovementComp->Velocity = -MovementComp->Velocity;
		ChangeInstigator(OtherActor);
		return;
	}
	
	// Apply Damage & Impulse
	if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, BaseDamage, SweepResult))
	{
		ProjectileHit(GetActorLocation());
	}
}
