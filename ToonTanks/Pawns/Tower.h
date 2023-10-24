// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "ToonTanks/Interfaces/DropItemInterface.h"
#include "Tower.generated.h"

class APickupBase;
/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn, public IDropItemInterface
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction_Implementation() override;
	virtual TArray<TSubclassOf<APickupBase>> GetItemsToDrop_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundBase* ActivationSound;
	UPROPERTY(EditAnywhere, Category = "Drop Items")
	TArray<TSubclassOf<APickupBase>> DropItems;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRange = 1200.f;
	UPROPERTY(EditANywhere, BlueprintReadWrite, Category="Combat")
	bool FireOnSightOnly = true;

private:
	UPROPERTY()
	class ATank* Tank;
	bool bActive = false;
	FVector OriginalRotation;

	void Activate(bool bNewActive);
	void CheckFireCondition();
	bool InFireRange();
	bool IsTargetVisible() const;
};
