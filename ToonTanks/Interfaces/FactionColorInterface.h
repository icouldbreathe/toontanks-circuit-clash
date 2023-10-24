// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FactionColorInterface.generated.h"

enum class EFaction : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFactionColorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOONTANKS_API IFactionColorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void ChangeFactionColor(EFaction Faction);
};
