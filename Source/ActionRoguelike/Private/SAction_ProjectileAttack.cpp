// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "CollisionDebugDrawingPublic.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarDebugDrawProjectile(TEXT("su.ProjectileDebugDraw"), false, TEXT("Enable Debug Lines for USAction_ProjectileAttack."), ECVF_Cheat);

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	CastSocketName = FName("Muzzle_01");
	AttackAnimDelay = 0.2f;
	AimRange = 5000.f;
	RageCost = 0.0f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Super::StartAction_Implementation(Instigator);
		
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), CastSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(CastSocketName);

		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(20.0f);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FVector TraceStart;
		FVector TraceEnd;
		GetTraceStartAndEnd(InstigatorCharacter->GetController(), TraceStart, TraceEnd);
		
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
	
		if (CVarDebugDrawProjectile.GetValueOnGameThread())
		{
			TArray<FHitResult> HitResults;
			HitResults.Add(Hit);
			DrawSphereSweeps(GetWorld(), TraceStart, TraceEnd, CollisionShape.GetSphereRadius(), HitResults, 4.f);
		}

		FRotator RotateTowards = FRotationMatrix::MakeFromX(TraceEnd - SpawnLocation).Rotator();
		FTransform SpawnTM = FTransform(RotateTowards, SpawnLocation);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		InstigatorCharacter->MoveIgnoreActorAdd(SpawnedActor);
	}

	StopAction(InstigatorCharacter);
}

void USAction_ProjectileAttack::GetTraceStartAndEnd(AController* Controller, FVector& Start, FVector& End)
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	Controller->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	FVector Target = ViewPointLocation + (ViewPointRotation.Vector() * AimRange);

	Start = ViewPointLocation;
	End = Target;
}

bool USAction_ProjectileAttack::CanStart_Implementation(AActor* Instigator)
{
	if (Super::CanStart_Implementation(Instigator))
	{
		if (RageCost > 0.0f)
		{
			ACharacter* Character = Cast<ACharacter>(Instigator);
			if (Character)
			{
				USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Character);
				return (AttributeComp && AttributeComp->ApplyRageChange(GetOwningComponent()->GetOwner(), -RageCost));
			}
		}

		return true;
	}

	return false;
}
