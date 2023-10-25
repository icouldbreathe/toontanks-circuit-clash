// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ToonTanks/Interfaces/SaveGameInterface.h"
#include "SaveGameData.generated.h"

enum class EFaction : uint8;

USTRUCT()
struct FGameSaveData
{
	GENERATED_USTRUCT_BODY()
	;

	FGameSaveData()
		: PlayerStart("0")
	{
	}

	UPROPERTY()
	FString PlayerStart;
	UPROPERTY()
	TMap<int32, EFaction> LevelStateMap;
};

/**
 * 
 */
UCLASS()
class TOONTANKS_API USaveGameData : public USaveGame, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	USaveGameData();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Save Data")
	FString SlotName;
	UPROPERTY()
	FString AppVersion;

	UPROPERTY()
	FGameSaveData GameSaveData = FGameSaveData();

	virtual bool SaveData_Implementation(UWorld* World) override;
	virtual void LoadData_Implementation(UWorld* World) override;
};
