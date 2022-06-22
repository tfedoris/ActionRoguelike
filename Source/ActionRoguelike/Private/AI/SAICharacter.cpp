// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"
#include "SWorldUserWidget.h"
#include "SWorldHealthBarWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

class ASPlayerState;
// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetMesh()->SetGenerateOverlapEvents(true);
	
	TimeToHitParamName = "TimeToHit";
	HitFlashColorParamName = "HitFlashColor";

	CreditsValue = 100;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	MulticastPawnSeenIndicator(Pawn);
	
	SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float ActualDelta, float Delta)
{
	if (ActualDelta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (!ActiveHealthBar)
		{
			ActiveHealthBar = CreateWidget<USWorldHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->StartingHealth = NewHealth;
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->WorldOffset = FVector(0.f, 0.f, BaseEyeHeight * 2.5f);
				ActiveHealthBar->AddToViewport();
				if (PawnSeenIndicator)
				{
					PawnSeenIndicator->WorldOffset = FVector(0.f, 0.f, BaseEyeHeight * 2.5f);
				}
			}	
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(1.0f, 0.0f, 0.0f));

		// Died
		if (NewHealth <= 0.0f)
		{
			// Grant Instigator Credits for the Kill
			ASPlayerState* InstigatorState = ASPlayerState::GetPlayerState(InstigatorActor);
			if (InstigatorState)
			{
				InstigatorState->AddCredits(CreditsValue);
			}
			
			// stop BT
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("Killed");
			}

			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("RagDoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			if (IsValid(PawnSeenIndicator))
			{
				PawnSeenIndicator->RemoveFromParent();
			}

			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

APawn* ASAICharacter::GetTargetActor()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		return Cast<APawn>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));	
	}

	return nullptr;
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::MulticastPawnSeenIndicator_Implementation(APawn* Pawn)
{
	if (!PawnSeenIndicator && GetTargetActor() != Pawn)
	{
		PawnSeenIndicator = CreateWidget<USWorldUserWidget>(GetWorld(), PawnSeenIndicatorClass);
		if (PawnSeenIndicator)
		{
			PawnSeenIndicator->AttachedActor = this;
			PawnSeenIndicator->WorldOffset = FVector(0.f, 0.f, BaseEyeHeight * 2.0f);
			PawnSeenIndicator->AddToViewport();
		}
	}
}
