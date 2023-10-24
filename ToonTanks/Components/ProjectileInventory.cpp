// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileInventory.h"

#include "ToonTanks/ToonTanksAssetManager.h"
#include "ToonTanks/Data/Items/ProjectileItem.h"

// Sets default values for this component's properties
UProjectileInventory::UProjectileInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

FPrimaryAssetId UProjectileInventory::GetEquippedSlotProjectileId() const
{
	if (Projectiles.IsValidIndex(EquippedSlot))
	{
		return Projectiles[EquippedSlot];
	}

	return FPrimaryAssetId("");
}

UProjectileItem* UProjectileInventory::GetEquippedProjectileItem() const
{
	if (!EquippedProjectileItem)
	{
		return Cast<UProjectileItem>(
			UToonTanksAssetManager::Get().GetPrimaryAssetObject(GetEquippedSlotProjectileId())
			);
	}

	return EquippedProjectileItem;
}

int32 UProjectileInventory::GetEquippedSlotIndex() const
{
	return EquippedSlot;
}

const TArray<FPrimaryAssetId>& UProjectileInventory::GetInventory() const
{
	return Projectiles;
}

void UProjectileInventory::ClearInventory()
{
	if (Projectiles.IsEmpty())
	{
		return;
	}

	Projectiles.Empty();
}

void UProjectileInventory::PopulateInventory(const TArray<FPrimaryAssetId>& NewInventory)
{
	ClearInventory();
	Projectiles.Append(NewInventory);
}

/**
 * Adds a projectile item to the inventory.
 * @param ProjectileId Projectile item id.
 * @return Index of the slot the projectile was added to or already exists. INDEX_NONE if item is invalid.
 */
int32 UProjectileInventory::AddProjectile(const FPrimaryAssetId& ProjectileId)
{
	if (!UToonTanksAssetManager::IsPrimaryAssetIdValid(ProjectileId) || ProjectileId.PrimaryAssetType !=
	    UToonTanksAssetManager::ProjectileItemType)
	{
		UE_LOG(LogTemp, Error, TEXT("Non valid asset id for projectile."))
		return INDEX_NONE;
	}

	int32 Index = Projectiles.IndexOfByKey(ProjectileId);

	if (Index != INDEX_NONE)
	{
		return Index;
	}

	Index = Projectiles.AddUnique(ProjectileId);

	if (OnInventoryChange.IsBound())
	{
		OnInventoryChange.Broadcast(Projectiles, Index);
	}

	return Index;
}

UProjectileItem* UProjectileInventory::EquipSlotWithOffset(const int32 IndexOffset)
{
	if (Projectiles.IsEmpty())
	{
		return nullptr;
	}

	int32 IndexWithOffset = EquippedSlot + IndexOffset;

	if (IndexWithOffset < 0)
	{
		IndexWithOffset = -IndexWithOffset * (Projectiles.Num() - 1);
	}

	return EquipSlot(IndexWithOffset % Projectiles.Num());
}

UProjectileItem* UProjectileInventory::EquipSlot(const int32 Index)
{
	if (EquippedSlot != Index && Projectiles.IsValidIndex(Index))
	{
		UProjectileItem* ProjectileItem = Cast<UProjectileItem>(
			UToonTanksAssetManager::Get().GetPrimaryAssetObject(Projectiles[Index])
			);

		if (!ProjectileItem)
		{
			return nullptr;
		}

		EquippedProjectileItem = ProjectileItem;

		if (OnSelectedProjectileSlotChange.IsBound())
		{
			OnSelectedProjectileSlotChange.Broadcast(Projectiles, Index, EquippedSlot);
		}

		EquippedSlot = Index;

		return EquippedProjectileItem;
	}

	return nullptr;
}

bool UProjectileInventory::IsEmpty() const
{
	return Projectiles.IsEmpty();
}

// Called when the game starts
void UProjectileInventory::BeginPlay()
{
	Super::BeginPlay();

	if (OnSelectedProjectileSlotChange.IsBound())
	{
		OnSelectedProjectileSlotChange.Broadcast(Projectiles, EquippedSlot, INDEX_NONE);
	}
}
