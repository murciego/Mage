// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPowerupActor.generated.h"

UCLASS()
class MAGE_API AMPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Intervalo de ticks
	UPROPERTY(EditDefaultsOnly, Category="Powerups")
	float PowerupInterval;

	// Veces aplicado el powerup
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNrOfTicks;

	FTimerHandle TimerHandle_PowerupTick;

	// Ticks aplicados 
	int32 TicksProcessed;

	UFUNCTION()
	void OnTickPowerup();

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
	bool bIsPowerupActive;

	UFUNCTION()
	void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChanged(bool bNewIsActive);

public:
	void ActivatePowerup(AActor *ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category ="Powerups")
	void OnActivated(AActor *ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category ="Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpired();
};
