// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/Tank.h"
#include "Pawns/Tower.h"
#include "Components/HealthComponent.h"
#include "ToonTanks/Interfaces/DropItemInterface.h"
#include "GameFramework/PlayerStart.h"
#include "SaveGame/SaveGameData.h"
#include "Actors/PickupBase.h"
#include "EngineUtils.h"
#include "ToonTanksAssetManager.h"
#include "ToonTanksCommon.h"
#include "ToonTanksGameInstance.h"
#include "ToonTanksUserSettings.h"
#include "Data/DifficultyConfig.h"
#include "Interfaces/SaveGameInterface.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (!DeadActor->Implements<UDestroyableInterface>())
	{
		DeadActor->Destroy();
		return;
	}

	if (DeadActor == Tank)
	{
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}

		GameOver(false);
	}
	else
	{
		DropItems(DeadActor);
	}

	IDestroyableInterface::Execute_HandleDestruction(DeadActor);
}

void AToonTanksGameMode::LevelCaptured(int32 LevelID, EFaction LevelState)
{
	LevelStateMap.Add(LevelID, LevelState);

	if (OnLevelCaptured.IsBound())
	{
		OnLevelCaptured.Broadcast(LevelID, LevelState);
	}
}

EFaction AToonTanksGameMode::GetCapturedState(int32 LevelID)
{
	if (const EFaction* State = LevelStateMap.Find(LevelID))
	{
		return *State;
	}

	return EFaction::Faction_None;
}

TMap<int32, EFaction> AToonTanksGameMode::GetLevelStates() const
{
	return LevelStateMap;
}

void AToonTanksGameMode::SetLevelStates(const TMap<int32, EFaction>& States)
{
	LevelStateMap = States;
}

void AToonTanksGameMode::SetPlayerStartTag(FString Tag)
{
	PlayerStartTag = Tag;
}

FString AToonTanksGameMode::GetPlayerStartTag() const
{
	return PlayerStartTag;
}

void AToonTanksGameMode::SetGameDifficulty(FPrimaryAssetId GameDifficultyAssetId)
{
	check(UToonTanksAssetManager::IsPrimaryAssetIdValid(GameDifficultyAssetId));
	UDifficultyConfig* Difficulty = Cast<UDifficultyConfig>(
		UToonTanksAssetManager::Get().GetPrimaryAssetObject(GameDifficultyAssetId));

	if (Difficulty)
	{
		GameDifficultyConfig = Difficulty;
		bEnableHealthBoostDrops = Difficulty->bDropHealthBoosts;
		HealthPercentToDropBoost = Difficulty->HealthPercentToDropBoost;
		HealthPickup = Difficulty->HealthPickup;
	}
}

UDifficultyConfig* AToonTanksGameMode::GetGameDifficulty() const
{
	return GameDifficultyConfig;
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	Cast<UToonTanksGameInstance>(GetGameInstance())->OnSaveGame.AddDynamic(this, &AToonTanksGameMode::SaveGame);
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();
}

AActor* AToonTanksGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	FString StartTag = UGameplayStatics::ParseOption(OptionsString, TEXT("start"));
	APlayerStart* FoundPlayerStart = nullptr;

	if (StartTag.IsEmpty() && SavedGame)
	{
		StartTag = PlayerStartTag;
	}

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (PlayerStart->bIsPIEPlayerStart)
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			FoundPlayerStart = PlayerStart;
			break;
		}

		if (PlayerStart->PlayerStartTag == FName(*StartTag))
		{
			FoundPlayerStart = PlayerStart;
		}
	}

	if (FoundPlayerStart)
	{
		return FoundPlayerStart;
	}

	if (UToonTanksCommon::IsEditor())
	{
		UE_LOG(LogTemp, Error, TEXT("Missing tagged player start! Will crash outside editor."))
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	check(FoundPlayerStart)

	return nullptr;
}

void AToonTanksGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	NewPlayer->SetName(DefaultPlayerName.ToString());
}

void AToonTanksGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	if (UGameplayStatics::DoesSaveGameExist(MapName, 0))
	{
		SavedGame = UGameplayStatics::LoadGameFromSlot(MapName, 0);
		ISaveGameInterface::Execute_LoadData(SavedGame, GetWorld());
	}
	else
	{
		SavedGame = UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass());
		ISaveGameInterface::Execute_SaveData(SavedGame, GetWorld());
	}

	SetGameDifficulty(UToonTanksUserSettings::GetToonTanksUserSettings()->GetGameDifficulty());

	Super::InitGame(MapName, Options, ErrorMessage);
}

void AToonTanksGameMode::SaveGame()
{
	if (SavedGame)
	{
		ISaveGameInterface::Execute_SaveData(SavedGame, GetWorld());
	}
}

void AToonTanksGameMode::DropItems(AActor* Actor) const
{
	TArray<TSubclassOf<APickupBase>> ItemsToDrop;

	if (bEnableHealthBoostDrops)
	{
		const auto HealthComp = Tank->FindComponentByInterface(UHealthInterface::StaticClass());
		float TankHealth, TankMaxHealth;
		IHealthInterface::Execute_GetActorHealth(HealthComp, TankHealth, TankMaxHealth);

		if (HealthPickup && TankHealth / TankMaxHealth <= HealthPercentToDropBoost)
		{
			ItemsToDrop.Add(HealthPickup);
		}
	}

	if (Actor->Implements<UDropItemInterface>())
	{
		ItemsToDrop.Append(IDropItemInterface::Execute_GetItemsToDrop(Actor));
	}

	for (auto Item : ItemsToDrop)
	{
		const FVector2D RandomPointInCircle = FMath::RandPointInCircle(DropItemRadius);

		FVector DropLocation = Actor->GetActorLocation();
		DropLocation.X += RandomPointInCircle.X;
		DropLocation.Y += RandomPointInCircle.Y;

		Tank->GetWorld()->SpawnActor<APickupBase>(Item, DropLocation, FRotator::ZeroRotator);
	}
}
