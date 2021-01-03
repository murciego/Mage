// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "core/MageGameUserSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "widgetConfig.generated.h"
class MageGameUserSettings;
/**
 * 
 */
UCLASS()
class MAGE_API UwidgetConfig : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My New User Widget")
		FString MyNewWidgetName;

	MageGameUserSettings* configuracion;
	UFUNCTION(BlueprintCallable)
		void init();
	UFUNCTION(BlueprintCallable)
		void SaveConfig();


	
};