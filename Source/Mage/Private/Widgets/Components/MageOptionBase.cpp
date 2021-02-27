// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Components/MageOptionBase.h"

#include "Widgets/widgetConfig.h"

void UMageOptionBase::SetOptions_Implementation(TArray<FString>& Opciones)
{
	
	MaxIndex = Opciones.Num()-1;
	Options = Opciones;
	UE_LOG(LogTemp, Warning, TEXT("SetOptions_Implementation"));
}


void UMageOptionBase::SetMageConfiguration_Implementation(FMageConfiguration Name)
{
	UE_LOG(LogTemp, Warning, TEXT("SetMageConfiguration_Implementation"));
}

