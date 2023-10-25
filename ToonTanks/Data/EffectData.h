// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToonTanks/ToonTanksAssetManager.h"
#include "EffectData.generated.h"

class UToonTanksAssetManager;
class AEffect;
/**
 * 
 */
UCLASS()
class TOONTANKS_API UEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effect")
	FPrimaryAssetType AssetType = UToonTanksAssetManager::EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FText EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	TSubclassOf<AEffect> EffectActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect Interaction")
	bool AllowMultiple = false;

	/** These effects will be neutralized when this effect is triggered. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect Interaction")
	TSet<FPrimaryAssetId> NeutralizeEffects;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(AssetType, GetFName());
	}
};
