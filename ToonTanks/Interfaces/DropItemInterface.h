// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DropItemInterface.generated.h"

class APickupBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDropItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOONTANKS_API IDropItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<TSubclassOf<APickupBase>> GetItemsToDrop();
};
