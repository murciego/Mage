// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class AMPowerupActor;

UCLASS()
class MAGE_API AMPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category ="Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly, Category="PickupActor")
	TSubclassOf<AMPowerupActor> PowerUpClass;

	AMPowerupActor *PowerUpInstance;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	float CooldownDuration;

	FTimerHandle TimeHandle_RespawnTimer;

	void Respawn();


public:
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
};
