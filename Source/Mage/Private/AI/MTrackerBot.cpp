/*****************************************************************//**
 * \file   MTrackerBot.cpp
 * \brief  AI que persigue al jugador más cercano
 * 
 * \author maxim
 * \date   January 2021
 *********************************************************************/
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
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"

#include "GameFramework/PlayerController.h"

static int32 DebugTrackerBotDrawing = 0;
FAutoConsoleVariableRef CVARDebugTrackerBotDrawing(
	TEXT("COOP.DebugTrackerBot"),
	DebugTrackerBotDrawing,
	TEXT("Draw Debug Lines for trackerbot"),
	ECVF_Cheat);

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

	ExplosionRadius = 350;
	ExplosionDamage = 60;
	SelftDamageInterval = 0.25f;
}

/**
 * Se inicia el timer TimerHandle_CheckPowerLevel en el servidor.
 * 
 */
void AMTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole() == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();

		FTimerHandle TimerHandle_CheckPowerLevel;

		GetWorldTimerManager().SetTimer(
			TimerHandle_CheckPowerLevel,
			 this,
			 &AMTrackerBot::OnCheckNearbyBots,
			 1.0f,
			 true);

	}
}
/**
 * Manejador del daño.
 * 
 * \param OwningHealthComp Componente de vida
 * \param Health Vida actual
 * \param HealthDelta 
 * \param DamageType Tipo de daño
 * \param InstigatedBy Instigador
 * \param DamageCauser Quien causa el daño
 */
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
	
	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}

/**
 * En el tick se van seleccionando los puntos en dirección al jugador mas cercano.
 * Si se alcanza un punto, se busca el siguiente. Así hasta que se alcance al jugador o muera.
 * 
 * \param DeltaTime
 */
void AMTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() == ROLE_Authority && !bExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
			if (DebugTrackerBotDrawing)
			{
				DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!", nullptr, FColor::White, 2.0f);
			}
			// UE_LOG(LogTemp, Warning, TEXT("Target Reached!"));
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;
			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
			
			// UE_LOG(LogTemp, Warning, TEXT("impulse"));
		}
		if(DebugTrackerBotDrawing) {
			DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
		}
	}
}
/**
 * Busca el siguiente punto en dirección al jugador y lo devuelve .
 * 
 * \return navPath point
 */
FVector AMTrackerBot::GetNextPathPoint()
{

	AActor *BestTarget = nullptr;
	float NearestTargetDistance = FLT_MAX;

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{

		AMageCharacter *PlayerPawn = Cast<AMageCharacter>(*It);
		if(!PlayerPawn){
			continue;
		}
		AActor *TestPawn = *It;
		// APawn *TestPawn = MyActor; //It->Get();
		if (TestPawn == nullptr || UMHealthComponent::IsFriendly(TestPawn, this))
		{
			continue;
		}

		UMHealthComponent *TestPawnHealthComp = 
			Cast<UMHealthComponent>(TestPawn->GetComponentByClass(UMHealthComponent::StaticClass()));

		// BestTarget = TestPawn;
		if(TestPawnHealthComp && TestPawnHealthComp->GetHealth() > 0.0f) {
			float Distance = (TestPawn->GetActorLocation() - GetActorLocation()).Size();

			GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);
			GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &AMTrackerBot::RefreshPath, 5.0f, false);

			if(Distance < NearestTargetDistance) {
				BestTarget = TestPawn;
				NearestTargetDistance = Distance;
			}
		}
	}
	if (BestTarget)
	{
		UNavigationPath *NavPath = UNavigationSystemV1::FindPathToActorSynchronously(
			this, 
			GetActorLocation(), 
			BestTarget
		);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			// Return next point in the path
			return NavPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}

/**
 * Pues eso, se autodestruye al tener al jugador cerca.
 * 
 */
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
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor *> IgnoredActors;
		IgnoredActors.Add(this);

		float ActualDamage = ExplosionDamage + (ExplosionDamage * PowerLevel);
		UGameplayStatics::ApplyRadialDamage(
			this,
			ActualDamage,
			GetActorLocation(),
			ExplosionRadius,
			nullptr,
			IgnoredActors,
			this,
			GetInstigatorController(),
			true);

		if (DebugTrackerBotDrawing)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		}
		SetLifeSpan(2.0f);
	}
}
/**
 * Evento que se lanca cuando un actor entra en rango.
 * 
 * \param OtherActor
 */
void AMTrackerBot::NotifyActorBeginOverlap(AActor *OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!bStartedSelfDestruction && !bExploded)
	{
		AMageCharacter *PlayerPawn = Cast<AMageCharacter>(OtherActor);

		if (PlayerPawn && !UMHealthComponent::IsFriendly(OtherActor, this))
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

/**
 * Se hace daño a modo de timer para la autodestrucción.
 * si le queda poca vida, tarda menos en explotar
 * 
 */
void AMTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}
/**
 * Comprueba los trackerbots en area para fortalecerse.
 * 
 */
void AMTrackerBot::OnCheckNearbyBots() {
	const float Radius = 600;

	FCollisionShape CollShape;
	CollShape.SetSphere(Radius);

	// Only Pawn
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, QueryParams,CollShape);

	if (DebugTrackerBotDrawing)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::White, false, 1.0f);
	}
	int32 NrOfBots = 0;

	for (FOverlapResult Result : Overlaps)
	{
		AMTrackerBot *Bot = Cast<AMTrackerBot>(Result.GetActor());
		if (Bot && Bot != this)
		{
			NrOfBots++;
		}
	}

	const int32 MaxPowerLevel = 4;

	PowerLevel = FMath::Clamp(NrOfBots, 0, MaxPowerLevel);

	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if (MatInst)
	{
		float Alpha = PowerLevel / (float)MaxPowerLevel;
		MatInst->SetScalarParameterValue("PowerLevelAlpha", Alpha);
	}

	if(DebugTrackerBotDrawing) {
		DrawDebugString(GetWorld(), FVector(0, 0, 0), FString::FromInt(PowerLevel), this, FColor::White, 1.0f, true);
	}
}
/**
 * Refresca el camino en caso de haberse quedado atascado.
 * 
 */
void AMTrackerBot::RefreshPath() {
	NextPathPoint = GetNextPathPoint();
}