// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileInventory.generated.h"

class UGameplayStatics;
class UProjectileItem;
class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChangeSignature, const TArray<FPrimaryAssetId>&, Inventory,
	int32, NewItemSlotIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSelectedProjectileSlotChangeSignature, const TArray<FPrimaryAssetId>&,
	Inventory, int32, SelectedSlotIndex, int32, PreviousSelectedSlotIndex);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UProjectileInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProjectileInventory();

	UFUNCTION(BlueprintPure)
	FPrimaryAssetId GetEquippedSlotProjectileId() const;
	UFUNCTION(BlueprintPure)
	UProjectileItem* GetEquippedProjectileItem() const;
	UFUNCTION(BlueprintPure)
	int32 GetEquippedSlotIndex() const;
	UFUNCTION(BlueprintCallable)
	const TArray<FPrimaryAssetId>& GetInventory() const;
	UFUNCTION(BlueprintCallable)
	void ClearInventory();
	UFUNCTION(BlueprintCallable)
	void PopulateInventory(const TArray<FPrimaryAssetId>& NewInventory);

	int32 AddProjectile(const FPrimaryAssetId& ProjectileId);
	UProjectileItem* EquipSlotWithOffset(const int32 IndexOffset);
	UFUNCTION(BlueprintCallable)
	UProjectileItem* EquipSlot(const int32 Index);
	bool IsEmpty() const;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryChangeSignature OnInventoryChange;
	UPROPERTY(BlueprintAssignable, Category="Projectile")
	FOnSelectedProjectileSlotChangeSignature OnSelectedProjectileSlotChange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPrimaryAssetId> Projectiles;

private:
	int32 EquippedSlot = 0;
	UPROPERTY()
	UProjectileItem* EquippedProjectileItem;
};
