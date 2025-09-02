#include "QTLoadingScreen/Public/QTLoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer/Public/MoviePlayer.h"

class FQTLoadingScreenModule : public IQTLoadingScreenModule
{
	virtual bool IsGameModule() const override{ return true; }
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime, UUserWidget* LoadingWidget) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.bMoviesAreSkippable = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		if (LoadingWidget)
		{
			TSharedPtr<SWidget> Widget = LoadingWidget->TakeWidget();
			LoadingScreen.WidgetLoadingScreen = Widget;
		}
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}
};

IMPLEMENT_GAME_MODULE(FQTLoadingScreenModule, QTLoadingScreen);