// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Data/FactionConfigData.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

class UDifficultyConfig;
class USaveGame;
class AToonTanksGameState;
class UToonTanksGameInstance;
class AToonTanksPlayerController;
class ATank;
class APickupBase;
class UToonTanksSaveGame;
class APlatformVolume;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelCapturedSignature, int32, LevelID, EFaction, Faction);

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);
	void LevelCaptured(int32 LevelID, EFaction LevelState);

	UFUNCTION(BlueprintCallable)
	void SetPlayerStartTag(FString Tag);
	UFUNCTION(BlueprintCallable)
	void SetGameDifficulty(FPrimaryAssetId GameDifficultyAssetId);
	void SetLevelStates(const TMap<int32, EFaction>& States);

	UFUNCTION(BlueprintPure)
	FString GetPlayerStartTag() const;
	EFaction GetCapturedState(int32 LevelID);
	UFUNCTION(BlueprintPure)
	TMap<int32, EFaction> GetLevelStates() const;
	UFUNCTION(BlueprintPure)
	UDifficultyConfig* GetGameDifficulty() const;

	UPROPERTY(BlueprintAssignable)
	FOnLevelCapturedSignature OnLevelCaptured;

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Drop")
	float DropItemRadius = 100.f;

private:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	void DropItems(AActor* Actor) const;

	UPROPERTY()
	ATank* Tank;
	UPROPERTY()
	AToonTanksPlayerController* ToonTanksPlayerController;
	UPROPERTY()
	USaveGame* SavedGame;
	UPROPERTY()
	UDifficultyConfig* GameDifficultyConfig;
	FString PlayerStartTag = "0";
	TMap<int32, EFaction> LevelStateMap;
	bool bEnableHealthBoostDrops = true;
	float HealthPercentToDropBoost = 0.5f;
	TSubclassOf<APickupBase> HealthPickup;
};
