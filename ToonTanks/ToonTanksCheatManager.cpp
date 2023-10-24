// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksCheatManager.h"

#include "ToonTanksAssetManager.h"
#include "ToonTanksCommon.h"
#include "ToonTanksPlayerController.h"
#include "Data/Items/HealthItem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

void UToonTanksCheatManager::ForceGarbageCollection()
{
	GEngine->ForceGarbageCollection(true);
}

void UToonTanksCheatManager::Give(const FPrimaryAssetId& ItemId) const
{
	AToonTanksPlayerController* PC = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (!UToonTanksAssetManager::IsPrimaryAssetIdValid(ItemId))
	{
		PC->ClientMessage("Invalid asset name.");
		return;
	}

	UObject* ItemObj = UToonTanksAssetManager::Get().GetPrimaryAssetObject(ItemId);
	if (!ItemObj)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (ItemId.PrimaryAssetType == UToonTanksAssetManager::ProjectileItemType)
	{
		UToonTanksCommon::AddProjectileToActor(PlayerPawn, ItemId);
		PRINT("Gave " + ItemId.PrimaryAssetName.ToString(), 3);
	}
	else if (ItemId.PrimaryAssetType == UToonTanksAssetManager::HealthItemType)
	{
		if (const UHealthItem* HealthItem = Cast<UHealthItem>(ItemObj))
		{
			UToonTanksCommon::AddHealthToActor(PlayerPawn, HealthItem->HealAmount);
			PRINT("Gave " + ItemId.PrimaryAssetName.ToString(), 3);
		}
	}
	else if (ItemId.PrimaryAssetType == UToonTanksAssetManager::EffectType)
	{
		UToonTanksCommon::SetEffectOnActor(PlayerPawn, ItemId, nullptr);
		PRINT("Gave " + ItemId.PrimaryAssetName.ToString(), 3);
	}
	else
	{
		PC->ClientMessage("Invalid asset type.");
	}
}

void UToonTanksCheatManager::DeleteSaves() const
{
	UGameplayStatics::DeleteGameInSlot(GetWorld()->GetMapName(), 0);
	UGameplayStatics::DeleteGameInSlot(
		UGameplayStatics::GetPlayerState(this, 0)->GetPlayerName(),
		0
		);

	PRINT("Deleted Saved Games", 3.0f);
}

void UToonTanksCheatManager::PossessTarget() const
{
	AToonTanksPlayerController* PC = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (APawn* Pawn = Cast<APawn>(HitResult.GetActor()))
	{
		PC->Possess(Pawn);
	}
}

void UToonTanksCheatManager::Unpossess()
{
	AToonTanksPlayerController* PC = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PC->UnPossess();
}
