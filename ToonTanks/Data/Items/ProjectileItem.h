// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ToonTanks/ToonTanksAssetManager.h"
#include "ProjectileItem.generated.h"

class UToonTanksAssetManager;
class AProjectile;
/**
 * 
 */
UCLASS()
class TOONTANKS_API UProjectileItem : public UItem
{
	GENERATED_BODY()

public:
	UProjectileItem()
	{
		ItemType = UToonTanksAssetManager::ProjectileItemType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float FireRate = 2.f;
};
