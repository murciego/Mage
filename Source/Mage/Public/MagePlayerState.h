// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MagePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API AMagePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category ="PlayerState")
	void AddScore(float ScoreDelta);
};
