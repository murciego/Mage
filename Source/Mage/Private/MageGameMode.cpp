// Fill out your copyright notice in the Description page of Project Settings.


#include "MageGameMode.h"
#include "Components/MHealthComponent.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "MageGameState.h"
#include "MagePlayerState.h"

AMageGameMode::AMageGameMode() {
    TimeBetweenWaves = 2.0f;
    NrOfBotsToSpawn = 2;

    GameStateClass = AMageGameState::StaticClass();
    PlayerStateClass = AMagePlayerState::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.0f;
}

void AMageGameMode::StartPlay()
{
    Super::StartPlay();
    PrepareForNextWave();
}

void AMageGameMode::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    CheckWaveState();
    CheckAnyPlayerAlive();
}

void AMageGameMode::StartWave() {
    NrOfBotsToSpawn = 2 * ++WaveCount;
    UE_LOG(LogTemp, Warning, TEXT("Next wave! %i"), NrOfBotsToSpawn);
    GetWorldTimerManager().SetTimer(
        TimerHandle_BotSpawner,
        this,
        &AMageGameMode::SpawnBotTimerElapsed,
        1.0f,
        true,
        0.0f);
    SetWaveState(EWaveState::WaveInProgress);
}


void AMageGameMode::SpawnBotTimerElapsed()
{
    SpawnNewBot();

    if(--NrOfBotsToSpawn <=0) {
        EndWave();
    }
    UE_LOG(LogTemp, Warning, TEXT("Spawn Timer Elapsed! bots  %i"), NrOfBotsToSpawn);
}


void AMageGameMode::EndWave()
{
    UE_LOG(LogTemp, Warning, TEXT("End wave"));

    GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
    SetWaveState(EWaveState::WaitingToComplete);
}
void AMageGameMode::PrepareForNextWave() {

    UE_LOG(LogTemp, Warning, TEXT("nexxt wave"));
    GetWorldTimerManager().SetTimer(
        TimerHandle_NextWaveStart,
        this,
        &AMageGameMode::StartWave,
        TimeBetweenWaves,
        false);
    SetWaveState(EWaveState::WaitingToStart);
}
void AMageGameMode::CheckWaveState() {

    UE_LOG(LogTemp, Warning, TEXT("check wave"));
    bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
    if (NrOfBotsToSpawn > 0 || bIsPreparingForWave) {
        return;
    }

    bool bIsAnyBoyAlive = false;
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
        // for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
    {
        APawn *TestPawn = Cast<APawn>(*It);
        if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
        {
            continue;
        }

        UMHealthComponent *HealthComp = Cast<UMHealthComponent>(TestPawn->GetComponentByClass(UMHealthComponent::StaticClass()));

        if (HealthComp && HealthComp->GetHealth() > 0.0f)
        {
            bIsAnyBoyAlive = true;
            break;
        }
    }

    if(!bIsAnyBoyAlive)
    {
        UE_LOG(LogTemp, Warning, TEXT("check ok"));

        SetWaveState(EWaveState::WaveComplete);
        PrepareForNextWave();
    }
}

void AMageGameMode::CheckAnyPlayerAlive() {
    for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();It;++It) {
        APlayerController *PC = It->Get();
        if(PC && PC->GetPawn()) {
            APawn* MyPawn = PC->GetPawn();
            UMHealthComponent *HealthComp = Cast<UMHealthComponent>
                (MyPawn->GetComponentByClass(UMHealthComponent::StaticClass()));
            if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f) {
                return; // Player still alive
            }
        }
    }
    // No player alive
    GameOver();
}

void AMageGameMode::SetWaveState(EWaveState NewState) {
    AMageGameState *GS = GetGameState<AMageGameState>();
    if(ensureAlways(GS)) {
        GS->SetWaveState(NewState);
    }
    
}

void AMageGameMode::GameOver()
{
    EndWave(); 
    // @TODO: Finish de match, mostrar GameOver a todos los jugadores

    SetWaveState(EWaveState::GameOver);
    UE_LOG(LogTemp, Warning, TEXT("GameOver! Players died"));
}