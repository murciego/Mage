// Fill out your copyright notice in the Description page of Project Settings.

#include "MageWeapon.h"
#include <Runtime\Engine\Public\DrawDebugHelpers.h>
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Mage/Mage.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraShakeBase.h"
#include "Chaos/ChaosEngineInterface.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for weapons"),
	ECVF_Cheat);
// Sets default values
AMageWeapon::AMageWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "BeamEnd";
	BaseDamage = 20.0f;
	BulletSpread = 2.0f;
	RateOfFire = 600;

	SetReplicates(true);
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void AMageWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}
void AMageWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMageWeapon::StartFire()
{

	// UE_LOG(LogTemp, Warning, TEXT("StartFire"));
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(
		TimerHandle_TimeBetweenShots,
		this,
		&AMageWeapon::Fire,
		TimeBetweenShots,
		true,
		FirstDelay);
}

void AMageWeapon::StopFire()
{
	// UE_LOG(LogTemp, Warning, TEXT("StopFire"));
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AMageWeapon::ServerFire_Implementation()
{
	// UE_LOG(LogTemp, Warning, TEXT("ServerFire"));
	Fire();
}
bool AMageWeapon::ServerFire_Validate()
{
	return true;
}
void AMageWeapon::OnRep_HitScanTrace()
{
	// Play cosmetic FX
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}
void AMageWeapon::Fire()
{

	if (GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}
	AActor *MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		// Ignores
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		// Mas exactitud
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		// ParticleTargetParam
		FVector TracerEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			// Hit
			AActor *HitActor = Hit.GetActor();
			UPhysicalMaterial *HitPhysMat = Hit.PhysMaterial.Get();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				UE_LOG(LogTemp, Warning, TEXT("HEAD SHOT"));
				ActualDamage *= 4.0f;
			}


			UGameplayStatics::ApplyPointDamage(
				HitActor,
				ActualDamage,
				ShotDirection,
				Hit,
				MyOwner->GetInstigatorController(),
				MyOwner,
				DamageType);
			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
			TracerEndPoint = Hit.ImpactPoint;
		}
		if (DebugWeaponDrawing > 0.0f)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 10.0f, 0, 2.0f);
		}
		PlayFireEffects(TracerEndPoint);
		if (GetLocalRole() == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}
		LastFireTime = GetWorld()->TimeSeconds;
	}
}
void AMageWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem *SelectedEffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
		SelectedEffect = FleshImpactEffect;
		break;
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}
	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}
void AMageWeapon::PlayFireEffects(FVector TracerEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent *TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}

	APawn *MyOwner = Cast<APawn>(GetOwner());

	if (MyOwner)
	{
		APlayerController *PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientStartCameraShake(FireCamShake);
		}
	}
}

void AMageWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AMageWeapon, HitScanTrace, COND_SkipOwner);
}