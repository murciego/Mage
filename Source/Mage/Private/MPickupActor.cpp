// Fill out your copyright notice in the Description page of Project Settings.


#include "MPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "MPowerupActor.h"
#include "TimerManager.h"

// Sets default values
AMPickupActor::AMPickupActor()
{

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMPickupActor::BeginPlay()
{
	Super::BeginPlay();
	Respawn();
}

void AMPickupActor::Respawn()
{
	if(PowerUpClass == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("PowerUpClass is nullptr in %s"), *GetName());
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerUpInstance = GetWorld()->SpawnActor<AMPowerupActor>(PowerUpClass, GetTransform(), SpawnParams);

}

void AMPickupActor::NotifyActorBeginOverlap(AActor *OtherActor) {
	
	Super::NotifyActorBeginOverlap(OtherActor);

	if(PowerUpInstance) {
		PowerUpInstance->ActivatePowerup();
		PowerUpInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimeHandle_RespawnTimer, this, &AMPickupActor::Respawn, CooldownDuration);

	}
}