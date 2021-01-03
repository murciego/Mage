// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/widgetConfig.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "core/MageGameUserSettings.h"


void UwidgetConfig::init()
{
	// configuracion = Cast<MageGameUserSettings >(GEngine->GetGameUserSettings());
	if (configuracion)
	{
		// configuration->SetMasterSoundVolume(15);
	}
}

void UwidgetConfig::SaveConfig()
{
	


	/*Scalability::SaveState(GGameUserSettingsIni);
	GEngine->GetGameUserSettings()->SaveSettings();*/
}

