// GPL-2.0 license


#include "ToonTanksGameInstance.h"

#include "ToonTanksUserSettings.h"
#include "ToonTanksAssetManager.h"
#include "Data/PostProcessData.h"
#include "Engine/AssetManager.h"
#include "Engine/PostProcessVolume.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "HttpModule.h"
#include "ToonTanksCommon.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"

void UToonTanksGameInstance::Init()
{
	Super::Init();

	AsyncCheckLatestVersion();

	UToonTanksUserSettings* Settings = UToonTanksUserSettings::GetToonTanksUserSettings();
	Settings->LoadSettings(true);
	Settings->ApplySettings(false);

	if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		AssetManager->LoadPrimaryAssetsWithType(UToonTanksAssetManager::ProjectileItemType);
		AssetManager->LoadPrimaryAssetsWithType(UToonTanksAssetManager::HealthItemType);
		AssetManager->LoadPrimaryAssetsWithType(UToonTanksAssetManager::EffectType);
		AssetManager->LoadPrimaryAssetsWithType(UToonTanksAssetManager::DifficultyConfigType);
	}

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UToonTanksGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UToonTanksGameInstance::EndLoadingScreen);
}

void UToonTanksGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		const auto LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass, TEXT("LoadingScreen"));
		LoadingSWidgetPtr = LoadingWidget->TakeWidget();

		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.WidgetLoadingScreen = LoadingSWidgetPtr;
		LoadingScreen.bAllowInEarlyStartup = false;
		LoadingScreen.PlaybackType = MT_Normal;
		LoadingScreen.bAllowEngineTick = false;
		LoadingScreen.bWaitForManualStop = false;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 1.f;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UToonTanksGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	// TODO: Implement Fade? Something else?
}

void UToonTanksGameInstance::SaveGame()
{
	if (OnSaveGame.IsBound())
	{
		OnSaveGame.Broadcast();
	}
}

void UToonTanksGameInstance::OverridePostProcessSettings(const FPostProcessSettings& InStruct)
{
	PPSettings = InStruct;
	if (PPVolumeActor)
	{
		PPVolumeActor->Settings = PPSettings;
	}
}

FPostProcessSettings UToonTanksGameInstance::GetPostProcessSettings() const
{
	return PPSettings;
}

void UToonTanksGameInstance::ApplyPostProcessSettings()
{
	if (!PostProcessData)
	{
		return;
	}

	PPSettings = PostProcessData->PPSettings;

	const auto UserSettings = UToonTanksUserSettings::GetToonTanksUserSettings();
	const float Gamma = UserSettings->GetGamma();
	const float Exp = UserSettings->GetBrightness();

	PPSettings.bOverride_ColorGamma = 1;
	PPSettings.ColorGamma = FVector4(Gamma, Gamma, Gamma, 1.f);
	PPSettings.bOverride_AutoExposureBias = 1;
	PPSettings.AutoExposureBias = Exp;

	if (PPVolumeActor)
	{
		PPVolumeActor->Settings = PPSettings;
	}
}

void UToonTanksGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);

	if (NewWorld)
	{
		PPVolumeActor = Cast<APostProcessVolume>(NewWorld->SpawnActor(APostProcessVolume::StaticClass()));
		PPVolumeActor->bEnabled = true;
		PPVolumeActor->bUnbound = true;
		PPVolumeActor->Priority = 1.f;

		ApplyPostProcessSettings();
	}
}

void UToonTanksGameInstance::AsyncCheckLatestVersion()
{
	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString("https://itch.io/api/1/x/wharf/latest?game_id=2318899&channel_name=windows"));
	Request->OnProcessRequestComplete().BindLambda([this](const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bSuccess)
	{
		if (bSuccess && Response->GetContentType() == "application/json")
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

			FJsonSerializer::Deserialize(JsonReader, JsonObject);

			ItchLatestVersion = JsonObject->GetStringField("latest");
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to GET latest version"));
		}
	});

	Request->ProcessRequest();
}

bool UToonTanksGameInstance::IsLatestVersion(FString& LatestVersion)
{
	FString AppVersion = UToonTanksCommon::GetAppVersion();

	if (AppVersion.Equals(ItchLatestVersion))
	{
		LatestVersion = ItchLatestVersion;
		return true;
	}

	if (ItchLatestVersion.Equals(UToonTanksCommon::GetBiggerVersion(AppVersion, ItchLatestVersion)))
	{
		LatestVersion = ItchLatestVersion;
		return false;
	}

	LatestVersion = AppVersion;
	return true;
}
