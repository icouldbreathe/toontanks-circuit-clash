// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DestroyableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDestroyableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOONTANKS_API IDestroyableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HandleDestruction();
};
