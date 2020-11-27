// Fill out your copyright notice in the Description page of Project Settings.

#include "MExplosiveBarrel.h"
#include "Components/MHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AMExplosiveBarrel::AMExplosiveBarrel()
{
	HealthComp = CreateDefaultSubobject<UMHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AMExplosiveBarrel::OnHealthChanged);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	ExplosionImpulse = 400;
}

// Called when the game starts or when spawned
void AMExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void AMExplosiveBarrel::OnHealthChanged(
	UMHealthComponent *OwningHealthComp,
	float Health,
	float HealthDelta,
	const class UDamageType *DamageType,
	class AController *InstigatedBy,
	AActor *DamageCauser)
{
	if (bExploded)
	{
		return;
	}
	if (Health <= 0.0f)
	{
		bExploded = true;
		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		MeshComp->AddImpulse(BoostIntensity, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		MeshComp->SetMaterial(0, ExplodedMaterial);

		RadialForceComp->FireImpulse();
	}
}