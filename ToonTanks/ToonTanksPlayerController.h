// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ToonTanksPlayerController.generated.h"

class USaveGame;
/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AToonTanksPlayerController();

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerEnabledState(bool bNewPlayerEnabled);
	UFUNCTION(BlueprintCallable)
	bool GetPlayerEnabledState() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerEnabled();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerDisabled();

	UFUNCTION(BlueprintCallable)
	void SavePlayer();

private:
	bool bPlayerEnabled;

	UPROPERTY()
	USaveGame* SavedPlayer;

protected:
	virtual void BeginPlay() override;
};
