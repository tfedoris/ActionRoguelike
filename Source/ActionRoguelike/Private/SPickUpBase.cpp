// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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
}

void ASPickUpBase::HandlePickUp(AActor* OtherActor)
{
	SetActorHiddenInGame(true);
	
	if (PickUpEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickUpEffect, GetActorLocation());
	}

	AudioComp->Play();
	
	GetWorldTimerManager().SetTimer(TimerHandle_HiddenDuration, this, &ASPickUpBase::OnHiddenDurationElapsed, RespawnDelay);
}

void ASPickUpBase::OnHiddenDurationElapsed()
{
	if (bCanRespawn)
	{
		SetActorHiddenInGame(false);
	}
	else
	{
		Destroy();
	}
}

void ASPickUpBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASPickUpBase::OnActorBeginOverlap);
}

void ASPickUpBase::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandlePickUp(OtherActor);
}
