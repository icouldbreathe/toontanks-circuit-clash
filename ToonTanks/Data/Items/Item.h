// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* ItemThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FLinearColor ItemColor = FLinearColor(FColor::White);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AActor> ItemActor;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(ItemType, GetFName());
	}
};
