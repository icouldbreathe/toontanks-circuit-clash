// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksAssetManager.h"

const FPrimaryAssetType UToonTanksAssetManager::ProjectileItemType = TEXT("Projectile");
const FPrimaryAssetType UToonTanksAssetManager::HealthItemType = TEXT("Health");
const FPrimaryAssetType UToonTanksAssetManager::EffectType = TEXT("Effect");
const FPrimaryAssetType UToonTanksAssetManager::DifficultyConfigType = TEXT("DifficultyConfig");

UToonTanksAssetManager& UToonTanksAssetManager::Get()
{
	UToonTanksAssetManager* This = Cast<UToonTanksAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be ToonTanksAssetManager!"));
	return *NewObject<UToonTanksAssetManager>(); // never calls this
}

bool UToonTanksAssetManager::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId)
{
	if (TArray<FPrimaryAssetId> IdList; Get().GetPrimaryAssetIdList(AssetId.PrimaryAssetType, IdList) && IdList.
	                                    Contains(AssetId))
	{
		return true;
	}
	return false;
}

UObject* UToonTanksAssetManager::ForceLoad(const FPrimaryAssetId& AssetId)
{
	const FSoftObjectPath AssetPath = Get().GetPrimaryAssetPath(AssetId);

	UObject* LoadedAsset = AssetPath.TryLoad();

	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed loading asset %s"), *AssetId.ToString());
	}

	return LoadedAsset;
}
