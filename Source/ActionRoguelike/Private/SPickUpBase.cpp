// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPickUpBase::ASPickUpBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = SphereComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetAutoActivate(false);
	AudioComp->SetupAttachment(RootComponent);

	RespawnDelay = 10.0f;
	bCanRespawn = true;
	bIsActive = true;

	SetReplicates(true);
}

void ASPickUpBase::HandlePickUp(AActor* OtherActor)
{
	ServerHandlePickup(OtherActor);
	ClientHandlePickupEffects();
}

void ASPickUpBase::ServerHandlePickup_Implementation(AActor* OtherActor)
{
	bIsActive = false;
	OnRep_IsActive();
	GetWorldTimerManager().SetTimer(TimerHandle_HiddenDuration, this, &ASPickUpBase::OnHiddenDurationElapsed, RespawnDelay);
}

void ASPickUpBase::ClientHandlePickupEffects_Implementation()
{
	if (PickUpEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickUpEffect, GetActorLocation());
	}

	AudioComp->Play();
}

void ASPickUpBase::OnHiddenDurationElapsed()
{
	if (!HasAuthority())
	{
		return;
	}
	if (bCanRespawn)
	{
		bIsActive = true;
		OnRep_IsActive();
	}
	else
	{
		SetLifeSpan(2.0f);
	}
}

void ASPickUpBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASPickUpBase::OnActorBeginOverlap);
}

void ASPickUpBase::OnRep_IsActive()
{
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
	
	SetActorEnableCollision(bIsActive);
}

void ASPickUpBase::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandlePickUp(OtherActor);
}

void ASPickUpBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickUpBase, bIsActive);
}
