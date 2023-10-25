// GPL-2.0 license


#include "ToonTanksUserSettings.h"

#include "ToonTanksAssetManager.h"

UToonTanksUserSettings::UToonTanksUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Brightness = 2.f;
	Gamma = 1.15f;
	bShowFPS = false;
	GameDifficulty = FPrimaryAssetId(UToonTanksAssetManager::DifficultyConfigType, "MediumDifficulty");
	bLevelStreaming = false;
}

void UToonTanksUserSettings::SetBrightness(float NewValue)
{
	if (!FMath::IsNearlyEqual(Brightness, NewValue))
	{
		bIsDirty = true;
		Brightness = NewValue;
	}
}

float UToonTanksUserSettings::GetBrightness() const
{
	return Brightness;
}

void UToonTanksUserSettings::SetGamma(float NewValue)
{
	if (!FMath::IsNearlyEqual(Gamma, NewValue))
	{
		bIsDirty = true;
		Gamma = NewValue;
	}
}

float UToonTanksUserSettings::GetGamma() const
{
	return Gamma;
}

void UToonTanksUserSettings::SetShowFps(bool NewValue)
{
	if (bShowFPS != NewValue)
	{
		bIsDirty = true;
		bShowFPS = NewValue;
	}
}

bool UToonTanksUserSettings::GetShowFps() const
{
	return bShowFPS;
}

void UToonTanksUserSettings::SetGameDifficulty(FPrimaryAssetId DifficultyID)
{
	if (GameDifficulty.ToString() != DifficultyID.ToString())
	{
		bIsDirty = true;
		GameDifficulty = DifficultyID;
	}
}

FPrimaryAssetId UToonTanksUserSettings::GetGameDifficulty() const
{
	return GameDifficulty;
}

void UToonTanksUserSettings::SetLevelStreaming(bool NewValue)
{
	if (bLevelStreaming != NewValue)
	{
		bIsDirty = true;
		bLevelStreaming = NewValue;
	}
}

bool UToonTanksUserSettings::GetLevelStreaming() const
{
	return bLevelStreaming;
}

bool UToonTanksUserSettings::IsDirty() const
{
	return Super::IsDirty() || bIsDirty;
}

void UToonTanksUserSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();

	if (GEngine->GetWorld())
	{
		if (bShowFPS)
		{
			GetWorld()->Exec(GetWorld(), TEXT("stat none|stat fps"));
		}
		else
		{
			GetWorld()->Exec(GetWorld(), TEXT("stat none"));
		}
	}

	bIsDirty = false;
}

void UToonTanksUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	Brightness = 2.f;
	Gamma = 1.15f;
	bShowFPS = false;
	bLevelStreaming = false;
}

UToonTanksUserSettings* UToonTanksUserSettings::GetToonTanksUserSettings()
{
	return Cast<UToonTanksUserSettings>(GetGameUserSettings());
}
