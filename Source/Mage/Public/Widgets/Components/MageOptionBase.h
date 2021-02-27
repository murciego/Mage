// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/widgetConfig.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MageOptionBase.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API UMageOptionBase : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, Category = "events")
		void SetOptions(TArray<FString>& Opciones);
	UFUNCTION(BlueprintNativeEvent, Category = "events")
		void SetMageConfiguration(FMageConfiguration config);
	// TArray<FText> Opciones

protected:

	UPROPERTY(BlueprintReadWrite)
		FString Titulo;
	UPROPERTY(BlueprintReadWrite)
		TArray<FString> Options;
	UPROPERTY(BlueprintReadWrite)
		uint8 Index;
	UPROPERTY(BlueprintReadWrite)
		uint8 MaxIndex;
};
