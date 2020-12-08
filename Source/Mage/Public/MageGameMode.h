// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "MageGameMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor *, VictimActor, AActor *, KillerActor, AController*, KillerController);
/**
 * 
 */
UCLASS()
class MAGE_API AMageGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	FTimerHandle TimerHandle_BotSpawner;
	
    FTimerHandle TimerHandle_NextWaveStart;

	int32 NrOfBotsToSpawn;
	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category= "GameMode")
	float TimeBetweenWaves;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();
	void SpawnBotTimerElapsed();
	
	void StartWave();


	void EndWave();
	void PrepareForNextWave();
	void CheckWaveState();
	void CheckAnyPlayerAlive();
	void GameOver();
	void SetWaveState(EWaveState NewState);

public:
	AMageGameMode();
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category="GameMode")
	FOnActorKilled OnActorKilled;
};
