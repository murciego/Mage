// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/widgetConfig.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "core/MageGameUserSettings.h"

/// Constructor.
/**
 * Inicializaci�n de la clase
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
	
	int i = 0;
	
	FMageConfiguration conf = FMageConfiguration();

	UE_LOG(LogTemp, Warning, TEXT("Set Resolution"));
	conf.index = i++;
	conf.configuration = EConfiguration::CONF_Resolution;
	conf.configType = EConfigType::CONFTYPE_Select;
	conf.configLevels = 3;
	conf.Name = FText::FromString("Resolution");
	mageConfigurations.Add(conf);

	UE_LOG(LogTemp, Warning, TEXT("Set AA"));
	conf = FMageConfiguration();
	conf.index = i++;
	conf.configuration = EConfiguration::CONF_AA;
	conf.configType = EConfigType::CONFTYPE_Select;
	conf.configLevels = 4;
	conf.Name = FText::FromString("AA");
	mageConfigurations.Add(conf);

}

/**
 * Inicializaci�n de la clase
 *
 * @param theory Even if there is only one possible unified theory. it is just a
 * @return void
 */
void UwidgetConfig::SaveConfig()
{
	


	/*Scalability::SaveState(GGameUserSettingsIni);
	GEngine->GetGameUserSettings()->SaveSettings();*/
}

