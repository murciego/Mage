// Fill out your copyright notice in the Description page of Project Settings.

#include "MageCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MageWeapon.h"
#include "Mage/Mage.h"
#include "Components/MHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMageCharacter::AMageCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<UMHealthComponent>(TEXT("HealthComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;
	bDied = false;
	WeaponAttachSocketName = "WeaponSocket";

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

// Called when the game starts or when spawned
void AMageCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

	HealthComp->OnHealthChanged.AddDynamic(this, &AMageCharacter::OnHealthChanged);
	if (GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<AMageWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		UE_LOG(LogTemp, Warning, TEXT("WeaponLog: Locale %f"), GetLocalRole());
		if (CurrentWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponLog: AttachWeapon"));
			CurrentWeapon->SetOwner(this);

			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}
}
void AMageCharacter::SetWeaponOwner_Implementation()
{
	CurrentWeapon->SetOwner(this);
}
void AMageCharacter::OnHealthChanged(
	UMHealthComponent *OwningHealthComp,
	float Health,
	float HealthDelta,
	const class UDamageType *DamageType,
	class AController *InstigatedBy,
	AActor *DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("HEALTH CHANGED EVENT %f"), Health);
	UE_LOG(LogTemp, Warning, TEXT("HEALTH CHANGED EVENT %b"), bDied);
	if (Health <= 0.0f && !bDied)
	{
		UE_LOG(LogTemp, Warning, TEXT("DIE EVENT"));
		// Die
		bDied = true;
		StopFire();
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);
	}
}
void AMageCharacter::StartFire()
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterFire"));
	if (CurrentWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartFire"));
		UE_LOG(LogTemp, Warning, TEXT("StartFire"));
		CurrentWeapon->StartFire();
	}
}

void AMageCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopFire"));
		UE_LOG(LogTemp, Warning, TEXT("StopFire"));
		CurrentWeapon->StopFire();
	}
}

// Called every frame
void AMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);
}
void AMageCharacter::MoveForward(float Value)
{

	// UE_LOG(LogTemp, Warning, TEXT("BeginRun %f"), Value);
	// Value = Value < 0 ? -.15 : Value;
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMageCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AMageCharacter::BeginCrouch()
{
	Crouch();
}

void AMageCharacter::EndCrouch()
{
	UnCrouch();
}
void AMageCharacter::BeginZoom()
{
	bWantsToZoom = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BeginZoom"));
	UE_LOG(LogTemp, Warning, TEXT("BeginZoom"));
}
void AMageCharacter::EndZoom()
{
	bWantsToZoom = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EndZoom"));
	UE_LOG(LogTemp, Warning, TEXT("EndZoom"));
}
void AMageCharacter::BeginRun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BeginRun"));
	UE_LOG(LogTemp, Warning, TEXT("BeginRun"));
}
void AMageCharacter::EndRun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EndRun"));
	UE_LOG(LogTemp, Warning, TEXT("EndRun"));
}
void AMageCharacter::Action()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Action"));
	UE_LOG(LogTemp, Warning, TEXT("Action"));
}
void AMageCharacter::Wheel(float Value)
{
}

void AMageCharacter::Menu_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Menu"));
	UE_LOG(LogTemp, Warning, TEXT("Menu"));
}

// Called to bind functionality to input
void AMageCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMageCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMageCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMageCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMageCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Wheel", this, &AMageCharacter::Wheel);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMageCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMageCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMageCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMageCharacter::BeginRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMageCharacter::EndRun);
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMageCharacter::Action);
	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &AMageCharacter::Menu);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMageCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMageCharacter::EndZoom);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMageCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMageCharacter::StopFire);
}

FVector AMageCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void AMageCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMageCharacter, CurrentWeapon);
	DOREPLIFETIME(AMageCharacter, bDied);
}