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

UENUM(BlueprintType)
enum class EConfiguration : uint8 {
	CONF_Resolution UMETA(DisplayName = "Resolución"),
	CONF_AA UMETA(DisplayName = "Antialiasing"),
	CONF_Shadows UMETA(DisplayName = "Sombras"),
	CONF_FPS UMETA(DisplayName = "FPS"),
	CONF_Textures UMETA(DisplayName = "Texturas"),
	CONF_Effects UMETA(DisplayName = "Efectos"),
	CONF_Detail UMETA(DisplayName = "Detalles"),
	CONF_FullScreen UMETA(DisplayName = "Pantalla completa")
};
UENUM(BlueprintType)
enum class EConfigType : uint8 {
	CONFTYPE_Select UMETA(DisplayName = "Select"),
	CONFTYPE_Boolean UMETA(DisplayName = "Check")
};
/**
 * struct,BlueprintType
 * Struct para almacenar los datos de las configuraciones
 * @author Jorge Murciego
 */
USTRUCT(BlueprintType)
struct FMageConfiguration
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		FText Name;//!< Nombre de la configuracion
	UPROPERTY(BlueprintReadWrite)
		EConfiguration configuration;
	UPROPERTY(BlueprintReadWrite)
		EConfigType configType;
	UPROPERTY(BlueprintReadWrite)
		uint8 configLevels;//!< 1(bajo), 2(bajo,medio)...
	UPROPERTY(BlueprintReadWrite)
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
	UPROPERTY(BlueprintReadWrite)
		TArray<FMageConfiguration> mageConfigurations;//!< BlueprintCallable, Struct de configuración

};