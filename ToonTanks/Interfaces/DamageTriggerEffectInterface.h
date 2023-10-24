// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageTriggerEffectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageTriggerEffectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOONTANKS_API IDamageTriggerEffectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TriggerEffect(AActor* DamagedActor, AController* Instigator);
};
