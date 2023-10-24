// Fill out your copyright notice in the Description page of Project Settings.

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
