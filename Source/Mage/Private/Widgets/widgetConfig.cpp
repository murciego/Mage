// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/widgetConfig.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "core/MageGameUserSettings.h"

/// Constructor.
/**
 * Inicialización de la clase
 *
 * @param theory Even if there is only one possible unified theory. it is just a
 * @return void
 */

void UwidgetConfig::init()
{
	// configuracion = Cast<MageGameUserSettings >(GEngine->GetGameUserSettings());
	if (configuracion)
	{
		// configuration->SetMasterSoundVolume(15);
	}
}

/**
 * Inicialización de la clase
 *
 * @param theory Even if there is only one possible unified theory. it is just a
 * @return void
 */
void UwidgetConfig::SaveConfig()
{
	


	/*Scalability::SaveState(GGameUserSettingsIni);
	GEngine->GetGameUserSettings()->SaveSettings();*/
}

