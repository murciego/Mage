// Fill out your copyright notice in the Description page of Project Settings.


#include "MageCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MageWeapon.h"

// Sets default values
AMageCharacter::AMageCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;
	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void AMageCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor < AMageWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrentWeapon) {
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);

	}
}
void AMageCharacter::Fire()
{
	if (CurrentWeapon) {
		CurrentWeapon->Fire();
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
void AMageCharacter::Menu()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Menu"));
	UE_LOG(LogTemp, Warning, TEXT("Menu"));
}


// Called to bind functionality to input
void AMageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMageCharacter::Fire);


}

FVector AMageCharacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

