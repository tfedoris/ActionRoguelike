// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	ProjectileDuration = 0.2f;
	TeleportDelay = 0.2f;

	MovementComp->InitialSpeed = 6000.0f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_ProjectileDuration, this, &ASTeleportProjectile::OnProjectileDurationElapsed, ProjectileDuration);
}

void ASTeleportProjectile::ProjectileHit(AActor* OtherActor, FVector HitLocation)
{
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().ClearTimer(TimerHandle_ProjectileDuration);
	
	if (PortalExitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalExitEffect, GetInstigator()->GetActorLocation(), GetInstigator()->GetActorRotation());
	}

	if (PortalEnterSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PortalEnterSoundCue, GetInstigator()->GetActorLocation());
	}
	
	if (PortalOpenEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalOpenEffect, GetActorLocation(), GetInstigator()->GetActorRotation());
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay, this, &ASTeleportProjectile::OnTeleportDelayEnd, TeleportDelay);
}

void ASTeleportProjectile::OnProjectileDurationElapsed()
{
	ProjectileHit(nullptr, GetActorLocation());
}

void ASTeleportProjectile::OnTeleportDelayEnd()
{
	TeleportInstigator();

	if (PortalExitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalExitEffect, GetInstigator()->GetActorLocation(), GetInstigator()->GetActorRotation());
	}

	if (PortalCloseEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalCloseEffect, GetActorLocation(), GetInstigator()->GetActorRotation());
	}

	if (PortalExitSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PortalExitSoundCue, GetActorLocation());
	}
	
	Destroy();
}

void ASTeleportProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	}
}
