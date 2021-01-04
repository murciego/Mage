// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MTrackerBot.generated.h"

class UMHealthComponent;
class USphereComponent;
class USoundCue;

/**
 * IA del trackerbot, un enemigo que va en dirección al jugador mas cercano
 * y explota al entrar en su rango de acción
 * @author Jorge Murciego
 */
UCLASS()
class MAGE_API AMTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	AMTrackerBot();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent *MeshComp;//!< VisibleDefaultsOnly, Category = "Components", Referencia al mesh

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent *SphereComp;//!< VisibleDefaultsOnly, Category = "Components", Collision 

	UFUNCTION()
	void HandleTakeDamage(
		UMHealthComponent *OwningHealthComp,
		float Health,
		float HealthDelta,
		const class UDamageType *DamageType,
		class AController *InstigatedBy,
		AActor *DamageCauser);//!< Manejador del daño

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UMHealthComponent *HealthComp;//!< VisibleDefaultsOnly, Category = "Components" Componente de la vida
	FVector GetNextPathPoint();//!< Devuelve el siguiente punto del navpath
	FVector NextPathPoint;//!< Siguiente punto del navPath
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce; //!< EditDefaultsOnly, Category = "TrackerBot" Fuerza de movimiento
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange; //!< EditDefaultsOnly, Category = "TrackerBot" 
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget; //!< EditDefaultsOnly, Category = "TrackerBot" Distancia minima para alcanzar el target

	// Material dynamic pulse on damage
	UMaterialInstanceDynamic *MatInst; //!< Material Dynamico que parpadea

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
		virtual void Tick(float DeltaTime) override;

		virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

protected:
	void OnCheckNearbyBots();
	int32 PowerLevel;
	FTimerHandle TimerHandle_RefreshPath;

	void RefreshPath();
};
