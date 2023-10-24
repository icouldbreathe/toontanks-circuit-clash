// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FactionConfigData.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Faction_None UMETA(DisplayName="None"),
	Faction_Tank UMETA(DisplayName="Tank"),
	Faction_Tower UMETA(DisplayName="Tower")
};

/**
 * 
 */
UCLASS()
class TOONTANKS_API UFactionConfigData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EFaction, FLinearColor> FactionColorMap;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Faction", GetFName());
	}
};
