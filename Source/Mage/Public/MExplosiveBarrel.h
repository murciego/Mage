// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MExplosiveBarrel.generated.h"

class UMHealthComponent;
class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;
// class UMaterialInterface;

UCLASS()
class MAGE_API AMExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMHealthComponent *HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent *RadialForceComp;

	UFUNCTION()
	void OnHealthChanged(
		UMHealthComponent *OwningHealthComp,
		float Health,
		float HealthDelta,
		const class UDamageType *DamageType,
		class AController *InstigatedBy,
		AActor *DamageCauser);

	bool bExploded;

	UPROPERTY(EditDefaultsOnly,
			  Category = "FX")
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem *ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UMaterialInterface *ExplodedMaterial;
};
