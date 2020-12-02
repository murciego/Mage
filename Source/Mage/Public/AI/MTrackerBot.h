// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MTrackerBot.generated.h"

class UMHealthComponent;
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
