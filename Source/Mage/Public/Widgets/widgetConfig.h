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
 * struct,BlueprintType
 * Struct para almacenar los datos de las configuraciones
 * @author Jorge Murciego
 */
USTRUCT(BlueprintType)
struct FMageConfig
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString Name;//!< Nombre de la configuracion
	UPROPERTY()
		FString conf;
	UPROPERTY()
		uint8 index;//!< Indice de la configuracion (bajo, medio, alto...)
};

/**
 * Clase para el widget de la configuracion
 * @author Jorge Murciego
 */
UCLASS()
class MAGE_API UwidgetConfig : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My New User Widget")
		FString MyNewWidgetName;//!< EditAnywhere, BlueprintReadWrite, Category = "My New User Widget", parametro de test

	MageGameUserSettings* configuracion;//!< GameUserSettings, configuración
	UFUNCTION(BlueprintCallable)
		void init();//!< BlueprintCallable, Constructor
	UFUNCTION(BlueprintCallable)
		void SaveConfig();//!< BlueprintCallable, Guarda la configuración
	UPROPERTY()
		FMageConfig mageConfiguration;//!< BlueprintCallable, Struct de configuración


	
};