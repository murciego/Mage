// Fill out your copyright notice in the Description page of Project Settings.


#include "MageCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

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


}

// Called when the game starts or when spawned
void AMageCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMageCharacter::MoveForward(float Value)
{
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

// Called every frame
void AMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMageCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMageCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMageCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMageCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMageCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMageCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMageCharacter::Jump);
}

FVector AMageCharacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

