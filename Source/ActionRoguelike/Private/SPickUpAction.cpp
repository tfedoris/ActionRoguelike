// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUpAction.h"

#include "SAction.h"
#include "SActionComponent.h"

ASPickUpAction::ASPickUpAction()
{
	bCanRespawn = false;
}

void ASPickUpAction::HandlePickUp(AActor* OtherActor)
{
	if (!ActionClass)
	{
		return;
	}
	
	USActionComponent* OtherActionComp = USActionComponent::GetActionComponent(OtherActor);
	if (OtherActionComp && !OtherActionComp->ContainsAction(ActionClass))
	{
		OtherActionComp->AddAction(OtherActor, ActionClass);
		Super::HandlePickUp(OtherActor);
	}
}
