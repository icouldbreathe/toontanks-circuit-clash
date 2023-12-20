// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ToonTanksGameInstance.generated.h"

class UPostProcessData;
class APostProcessVolume;
class UToonTanksSaveGame;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveGameSignature);

/**
 * 
 */
UCLASS()
class TOONTANKS_API UToonTanksGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loading Screen")
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Post Process")
	UPostProcessData* PostProcessData;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void OverridePostProcessSettings(const FPostProcessSettings& InStruct);
	UFUNCTION(BlueprintPure)
	FPostProcessSettings GetPostProcessSettings() const;
	UFUNCTION(BlueprintCallable)
	void ApplyPostProcessSettings();

	UFUNCTION(BlueprintCallable)
	bool IsLatestVersion(FString& LatestVersion);

	UPROPERTY(BlueprintAssignable)
	FSaveGameSignature OnSaveGame;

	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

private:
	void AsyncCheckLatestVersion();

	UPROPERTY()
	APostProcessVolume* PPVolumeActor;
	UPROPERTY()
	FPostProcessSettings PPSettings;
	TSharedPtr<SWidget> LoadingSWidgetPtr;

	FString ItchLatestVersion;
};
