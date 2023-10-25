// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "BaseFactionActor.h"
#include "FactionPlatform.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AFactionPlatform : public ABaseFactionActor
{
	GENERATED_BODY()

public:
	AFactionPlatform();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Corners")
	UStaticMesh* CornerMesh;
	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* CornerMeshInstanced;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Bitmask, BitmaskEnum = "/Script/ToonTanks.ECornerFlags"),
		Category="Corners")
	uint8 BitmaskCornerFlags = 0;
};
