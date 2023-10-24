// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/ToonTanksAssetManager.h"
#include "DifficultyConfig.generated.h"

class APickupBase;
class UToonTanksAssetManager;
/**
 * 
 */
UCLASS()
class TOONTANKS_API UDifficultyConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	FPrimaryAssetType DifficultyConfigType = UToonTanksAssetManager::DifficultyConfigType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game")
	bool bDropHealthBoosts = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game")
	float HealthPercentToDropBoost = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game")
	TSubclassOf<APickupBase> HealthPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tank")
	float TankReceiveDamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tower")
	float TowerReceiveDamageMultiplier = 1.f;

	float GetFinalDamage(float Damage, AActor* DamagedActor, AController* Instigator, const UDamageType* DamageType) const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(DifficultyConfigType, GetFName());
	}
};
