// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MageCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AMageWeapon;
class UMHealthComponent;

UCLASS()
class MAGE_API AMageCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMageCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent *CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent *SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMHealthComponent *HealthComp;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;
	bool bWantsToZoom;
	float DefaultFOV;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void BeginCrouch();
	void EndCrouch();
	void BeginRun();
	void EndRun();
	void BeginZoom();
	void EndZoom();
	void Action();
	void Wheel(float Value);

	UPROPERTY(Replicated)
	AMageWeapon *CurrentWeapon;

	UFUNCTION(Client, Reliable)
	void SetWeaponOwner();

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AMageWeapon> StarterWeaponClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;
	UFUNCTION()
	void OnHealthChanged(
		UMHealthComponent *OwningHealthComp,
		float Health,
		float HealthDelta,
		const class UDamageType *DamageType,
		class AController *InstigatedBy,
		AActor *DamageCauser);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;

public:
	// Called every frame
	virtual void
	Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();
	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Menu();

};
