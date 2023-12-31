// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputRotateTurret;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputTurn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputChangeProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputSlot;
};
