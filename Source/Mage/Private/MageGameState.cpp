// Fill out your copyright notice in the Description page of Project Settings.


#include "MageGameState.h"
#include "Net/UnrealNetwork.h"

void AMageGameState::OnRep_WaveState(EWaveState OldState)
{
    WaveStateChanged(WaveState, OldState);
}
void AMageGameState::SetWaveState(EWaveState NewState)
{
    if(GetLocalRole() == ROLE_Authority) {
        EWaveState OldState = WaveState;
        WaveState = NewState;
        OnRep_WaveState(OldState);
    }
}
void AMageGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMageGameState, WaveState );
}