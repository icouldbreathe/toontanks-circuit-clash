// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GravityInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGravityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOONTANKS_API IGravityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableGravity(bool bEnabled);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsFalling();
};
