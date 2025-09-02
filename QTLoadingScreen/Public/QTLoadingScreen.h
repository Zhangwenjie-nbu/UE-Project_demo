#pragma once
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IQTLoadingScreenModule : public IModuleInterface
{
public:
	static inline IQTLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IQTLoadingScreenModule>("QTLoadingScreen");
	}
	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime, UUserWidget* LoadingWidget) = 0;

	/** Stops the loading screen */
	virtual void StopInGameLoadingScreen() = 0;
};
