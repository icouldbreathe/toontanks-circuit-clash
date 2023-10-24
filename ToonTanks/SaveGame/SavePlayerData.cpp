// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePlayerData.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/ToonTanksCommon.h"
#include "ToonTanks/Components/ProjectileInventory.h"

USavePlayerData::USavePlayerData()
{
	PlayerSaveData = FPlayerSaveData();
}

bool USavePlayerData::SaveData_Implementation(UWorld* World)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);

	if (!PlayerController)
	{
		return false;
	}

	SlotName = PlayerController->GetPlayerState<APlayerState>()->GetPlayerName();
	AppVersion = UToonTanksCommon::GetAppVersion();

	const UProjectileInventory* PlayerInventory = Cast<UProjectileInventory>(
		PlayerController->GetPawn()->GetComponentByClass(UProjectileInventory::StaticClass())
		);

	if (!PlayerInventory)
	{
		return false;
	}

	FPlayerSaveData NewPlayerData;

	NewPlayerData.Inventory = PlayerInventory->GetInventory();
	NewPlayerData.InventorySlot = PlayerInventory->GetEquippedSlotIndex();

	PlayerSaveData = NewPlayerData;

	return UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}

void USavePlayerData::LoadData_Implementation(UWorld* World)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);

	if (!PlayerController)
	{
		return;
	}

	UProjectileInventory* PlayerInventory = Cast<UProjectileInventory>(
		PlayerController->GetPawn()->GetComponentByClass(UProjectileInventory::StaticClass())
		);

	PlayerInventory->PopulateInventory(PlayerSaveData.Inventory);
	PlayerInventory->EquipSlot(PlayerSaveData.InventorySlot);
}
