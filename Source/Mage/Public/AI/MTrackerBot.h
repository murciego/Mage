// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MTrackerBot.generated.h"

class UMHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class MAGE_API AMTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent *SphereComp;

	UFUNCTION()
	void HandleTakeDamage(
		UMHealthComponent *OwningHealthComp,
		float Health,
		float HealthDelta,
		const class UDamageType *DamageType,
		class AController *InstigatedBy,
		AActor *DamageCauser);

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UMHealthComponent *HealthComp;
	FVector GetNextPathPoint();
	FVector NextPathPoint;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	// Material dynamic pulse on damage
	UMaterialInstanceDynamic *MatInst;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem *ExplosionEffect;
	void SelfDestruct();
	bool bExploded;
	bool bStartedSelfDestruction;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelftDamageInterval;

	FTimerHandle TimerHandle_SelfDamage;
	void DamageSelf();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue *SelfDestructSound;
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* ExplodeSound;
public :
		// Called every frame
		virtual void
		Tick(float DeltaTime) override;

		virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

protected:
	void OnCheckNearbyBots();
	int32 PowerLevel;
	FTimerHandle TimerHandle_RefreshPath;

	void RefreshPath();
};
