// Fill out your copyright notice in the Description page of Project Settings.


#include "MPowerupActor.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AMPowerupActor::AMPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNrOfTicks = 0;
	bIsPowerupActive = false;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AMPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMPowerupActor::ActivatePowerup(AActor *ActivateFor) {
	OnActivated(ActivateFor);

	bIsPowerupActive = true;
	OnRep_PowerupActive();
	
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

void AMPowerupActor::OnRep_PowerupActive() {
	OnPowerupStateChanged(bIsPowerupActive);
}

void AMPowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTicked();
	
	if (TicksProcessed >= TotalNrOfTicks) {

		OnExpired();

		bIsPowerupActive = false;
		OnRep_PowerupActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void AMPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMPowerupActor, bIsPowerupActive);
}
