// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Kismet/GameplayStatics.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	ProjectileDuration = 0.2f;
	TeleportDelay = 0.2f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_ProjectileDuration, this, &ASTeleportProjectile::OnProjectileDurationElapsed, ProjectileDuration);
}

void ASTeleportProjectile::ProjectileHit(FVector HitLocation)
{
	Super::ProjectileHit(HitLocation);

	if (PortalExitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalExitEffect, GetInstigator()->GetActorLocation(), GetInstigator()->GetActorRotation());
	}
	
	if (PortalOpenEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalOpenEffect, GetActorLocation(), GetInstigator()->GetActorRotation());
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay, this, &ASTeleportProjectile::OnTeleportDelayEnd, TeleportDelay);
}

void ASTeleportProjectile::OnProjectileDurationElapsed()
{
	ProjectileHit(GetActorLocation());
}

void ASTeleportProjectile::OnTeleportDelayEnd()
{
	GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());

	if (PortalExitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalExitEffect, GetInstigator()->GetActorLocation(), GetInstigator()->GetActorRotation());
	}

	if (PortalCloseEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalCloseEffect, GetActorLocation(), GetInstigator()->GetActorRotation());
	}
	
	Destroy();
}
