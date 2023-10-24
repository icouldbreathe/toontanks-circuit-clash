// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EffectInterface.generated.h"

class AEffect;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEffectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOONTANKS_API IEffectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AEffect* TriggerEffect(const FPrimaryAssetId& EffectId, const AController* Instigator, bool bDeferred);
};
