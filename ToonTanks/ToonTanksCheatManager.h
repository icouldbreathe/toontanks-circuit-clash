// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "ToonTanksCheatManager.generated.h"

class AToonTanksPlayerController;
/**
 * 
 */
UCLASS()
class TOONTANKS_API UToonTanksCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void ForceGarbageCollection();

	UFUNCTION(Exec)
	void Give(const FPrimaryAssetId& ItemId) const;

	UFUNCTION(Exec)
	void DeleteSaves() const;

	UFUNCTION(Exec)
	void PossessTarget() const;

	UFUNCTION(Exec)
	void Unpossess();
};
