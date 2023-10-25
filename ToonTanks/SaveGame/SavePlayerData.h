// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ToonTanks/Interfaces/SaveGameInterface.h"
#include "SavePlayerData.generated.h"

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()
	;

	FPlayerSaveData()
		: Inventory({FPrimaryAssetId("Projectile", "Projectile")})
		, InventorySlot(0)
	{
	}

	UPROPERTY()
	TArray<FPrimaryAssetId> Inventory = {};
	UPROPERTY()
	int32 InventorySlot = INDEX_NONE;
};

/**
 * 
 */
UCLASS()
class TOONTANKS_API USavePlayerData : public USaveGame, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	USavePlayerData();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Save Data")
	FString SlotName;
	UPROPERTY()
	FString AppVersion;

	UPROPERTY()
	FPlayerSaveData PlayerSaveData = FPlayerSaveData();

	virtual bool SaveData_Implementation(UWorld* World) override;
	virtual void LoadData_Implementation(UWorld* World) override;
};
