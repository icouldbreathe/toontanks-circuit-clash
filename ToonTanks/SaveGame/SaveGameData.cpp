// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameData.h"

#include "ToonTanks/ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/ToonTanksCommon.h"

USaveGameData::USaveGameData()
{
	GameSaveData = FGameSaveData();
}

bool USaveGameData::SaveData_Implementation(UWorld* World)
{
	const AToonTanksGameMode* GameMode = Cast<AToonTanksGameMode>(World->GetAuthGameMode());

	if (!GameMode)
	{
		return false;
	}

	SlotName = World->GetMapName();
	AppVersion = UToonTanksCommon::GetAppVersion();

	FGameSaveData NewGameData;

	NewGameData.PlayerStart = GameMode->GetPlayerStartTag();
	NewGameData.LevelStateMap = GameMode->GetLevelStates();

	GameSaveData = NewGameData;

	return UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}

void USaveGameData::LoadData_Implementation(UWorld* World)
{
	AToonTanksGameMode* GameMode = Cast<AToonTanksGameMode>(World->GetAuthGameMode());

	if (!GameMode)
	{
		return;
	}

	GameMode->SetPlayerStartTag(GameSaveData.PlayerStart);
	GameMode->SetLevelStates(GameSaveData.LevelStateMap);
}
