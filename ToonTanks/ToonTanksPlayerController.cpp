// GPL-2.0 license


#include "ToonTanksPlayerController.h"

#include "ToonTanksCheatManager.h"
#include "ToonTanksGameInstance.h"
#include "Interfaces/SaveGameInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SavePlayerData.h"
#include "GameFramework/PlayerState.h"


AToonTanksPlayerController::AToonTanksPlayerController()
{
	CheatClass = UToonTanksCheatManager::StaticClass();
	bPlayerEnabled = false;
}

void AToonTanksPlayerController::SetPlayerEnabledState(bool bNewPlayerEnabled)
{
	bPlayerEnabled = bNewPlayerEnabled;

	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
		SetIgnoreLookInput(false);
		OnPlayerEnabled();
	}
	else
	{
		GetPawn()->DisableInput(this);
		SetIgnoreLookInput(true);
		OnPlayerDisabled();
	}
	bShowMouseCursor = bPlayerEnabled;
}

bool AToonTanksPlayerController::GetPlayerEnabledState() const
{
	return bPlayerEnabled;
}

void AToonTanksPlayerController::SavePlayer()
{
	if (SavedPlayer)
	{
		ISaveGameInterface::Execute_SaveData(SavedPlayer, GetWorld());
	}
}

void AToonTanksPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Cast<UToonTanksGameInstance>(GetGameInstance())->OnSaveGame.AddDynamic(
		this, &AToonTanksPlayerController::SavePlayer);

	if (UGameplayStatics::DoesSaveGameExist(PlayerState->GetPlayerName(), 0))
	{
		SavedPlayer = UGameplayStatics::LoadGameFromSlot(PlayerState->GetPlayerName(), 0);
	}
	else
	{
		SavedPlayer = UGameplayStatics::CreateSaveGameObject(USavePlayerData::StaticClass());
	}

	ISaveGameInterface::Execute_LoadData(SavedPlayer, GetWorld());
}
