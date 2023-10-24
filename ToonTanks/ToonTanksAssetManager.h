// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ToonTanksAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UToonTanksAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UToonTanksAssetManager()
	{
	}

	static UToonTanksAssetManager& Get();

	static const FPrimaryAssetType ProjectileItemType;
	static const FPrimaryAssetType HealthItemType;
	static const FPrimaryAssetType EffectType;
	static const FPrimaryAssetType DifficultyConfigType;

	static bool IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId);
	static UObject* ForceLoad(const FPrimaryAssetId& AssetId);
};
