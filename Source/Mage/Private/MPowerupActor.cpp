// Fill out your copyright notice in the Description page of Project Settings.


#include "MPowerupActor.h"

// Sets default values
AMPowerupActor::AMPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNrOfTicks = 0;
}

// Called when the game starts or when spawned
void AMPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMPowerupActor::ActivatePowerup() {
	OnActivated();
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle_PowerupTick,
			this, &AMPowerupActor::OnTickPowerup,
			PowerupInterval,
			true);
	}
	else
	{
		OnTickPowerup();
	}
}

void AMPowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTicked();
	
	if (TicksProcessed >= TotalNrOfTicks) {

		OnExpired();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}
