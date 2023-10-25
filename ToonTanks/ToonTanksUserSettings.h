// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ToonTanksUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UToonTanksUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetBrightness(float NewValue);
	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetBrightness() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetGamma(float NewValue);
	UFUNCTION(BlueprintPure, Category = "Settings")
	float GetGamma() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetShowFps(bool NewValue);
	UFUNCTION(BlueprintPure, Category = "Settings")
	bool GetShowFps() const;
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetGameDifficulty(FPrimaryAssetId DifficultyID);
	UFUNCTION(BlueprintPure, Category = "Settings")
	FPrimaryAssetId GetGameDifficulty() const;
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetLevelStreaming(bool NewValue);
	UFUNCTION(BlueprintPure, Category = "Settings")
	bool GetLevelStreaming() const;

	virtual bool IsDirty() const override;
	virtual void ApplyNonResolutionSettings() override;
	virtual void SetToDefaults() override;
	UFUNCTION(BlueprintCallable)
	static UToonTanksUserSettings* GetToonTanksUserSettings();

protected:
	UPROPERTY(Config)
	float Brightness;
	UPROPERTY(Config)
	float Gamma;
	UPROPERTY(Config)
	bool bShowFPS;
	UPROPERTY(Config)
	FPrimaryAssetId GameDifficulty;
	UPROPERTY(Config)
	bool bLevelStreaming;

private:
	bool bIsDirty = false;
};
