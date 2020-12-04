// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MTrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Components/MHealthComponent.h"
#include "MageCharacter.h"
#include "Components/MHealthComponent.h"
#include "EngineUtils.h"
#include "MageCharacter.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"

#include "GameFramework/PlayerController.h"
// Sets default values
AMTrackerBot::AMTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
	bUseVelocityChange = false;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;

	HealthComp = CreateDefaultSubobject<UMHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AMTrackerBot::HandleTakeDamage);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bExploded = false;

	ExplosionRadius = 200;
	ExplosionDamage = 40;
	SelftDamageInterval = 0.25f;
}

// Called when the game starts or when spawned
void AMTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole() == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();
	}
}

void AMTrackerBot::HandleTakeDamage(
	UMHealthComponent *OwningHealthComp,
	float Health,
	float HealthDelta,
	const class UDamageType *DamageType,
	class AController *InstigatedBy,
	AActor *DamageCauser)
{

	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());
	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}
// Called every frame
void AMTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() == ROLE_Authority && !bExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
			DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!", nullptr, FColor::White, 2.0f);
			UE_LOG(LogTemp, Warning, TEXT("Target Reached!"));
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;
			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("impulse"));
		}
		DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
	}
}

FVector AMTrackerBot::GetNextPathPoint()
{

	// APlayerController *PC = GetPlayerController(this, 0);

	// ACharacter *PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	/*
	UNavigationPath *NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
	if (NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}
	UE_LOG(LogTemp, Warning, TEXT("No next path point"));
*/

	AActor *BestTarget = nullptr;

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{

		AActor *TestPawn = *It;
		// APawn *TestPawn = MyActor; //It->Get();
		if (TestPawn == nullptr /* || UMHealthComponent::IsFriendly(TestPawn, this)*/)
		{
			continue;
		}

		UMHealthComponent *TestPawnHealthComp = Cast<UMHealthComponent>(TestPawn->GetComponentByClass(UMHealthComponent::StaticClass()));

		BestTarget = TestPawn;
	}
	if (BestTarget)
	{
		UNavigationPath *NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			// Return next point in the path
			return NavPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}

void AMTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}
	bExploded = true;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	
	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor *> IgnoredActors;
		IgnoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		SetLifeSpan(2.0f);
	}
}

void AMTrackerBot::NotifyActorBeginOverlap(AActor *OtherActor)
{
	if (!bStartedSelfDestruction && !bExploded)
	{
		AMageCharacter *PlayerPawn = Cast<AMageCharacter>(OtherActor);

		if (PlayerPawn)
		{

			if (GetLocalRole() == ROLE_Authority)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &AMTrackerBot::DamageSelf, SelftDamageInterval, true);
			}
			bStartedSelfDestruction = true;
			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}
void AMTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}