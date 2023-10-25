// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PostProcessData.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UPostProcessData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Post Process")
	FPostProcessSettings PPSettings;
};
