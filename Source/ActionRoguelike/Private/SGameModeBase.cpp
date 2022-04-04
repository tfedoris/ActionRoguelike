// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SCoin.h"
#include "SHealthPotion.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

// Prefix 'su' here is just specifying our own category, in this case it stands for "Stanford University"
// Marking this with the ECVF_Cheat flag will exclude it from the release build
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugDrawGameMode(TEXT("su.GameModeDebugDraw"), false, TEXT("Enable Debug Lines for Game Mode functionality."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	RespawnDelay = 2.0f;
	NumCoinsToSpawn = 10;
	NumPotionsToSpawn = 5;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnPickUps();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it's allowed to spawn is determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // TODO: maybe pass in player for kill credit?
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int32 NumAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NumAliveBots);

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NumAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotQueryCompleted);	
	}
}

void ASGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}
	
	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		if (CVarDebugDrawGameMode.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
		}
	}
}

void ASGameModeBase::OnSpawnCoinQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn pick ups EQS Query failed!"));
		return;
	}
	
	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(CoinClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnSpawnPotionQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn pick ups EQS Query failed!"));
		return;
	}
	
	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(HealthPotionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(!ensure(Controller))
	{
		return;
	}

	Controller->UnPossess();
	RestartPlayer(Controller);
}

void ASGameModeBase::SpawnPickUps()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance;
	
	for(int i = 0; i < NumCoinsToSpawn; i++)
	{
		QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPickUpsQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnCoinQueryCompleted);
		}
	}

	for(int i = 0; i < NumPotionsToSpawn; i++)
	{
		QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPickUpsQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnPotionQueryCompleted);
		}
	}
}
