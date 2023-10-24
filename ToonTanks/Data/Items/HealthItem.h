// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ToonTanks/ToonTanksAssetManager.h"
#include "HealthItem.generated.h"

class UToonTanksAssetManager;
class APickupBase;
/**
 * 
 */
UCLASS()
class TOONTANKS_API UHealthItem : public UItem
{
	GENERATED_BODY()

public:
	UHealthItem()
	{
		ItemType = UToonTanksAssetManager::HealthItemType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float HealAmount = 10.f;
};
