// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MageWeapon.h"
#include "MageProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API AMageProjectileWeapon : public AMageWeapon
{
	GENERATED_BODY()

protected:
	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category="ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;
};
