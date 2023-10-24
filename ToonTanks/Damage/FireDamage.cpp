// Fill out your copyright notice in the Description page of Project Settings.


#include "FireDamage.h"

#include "ToonTanks/ToonTanksAssetManager.h"
#include "ToonTanks/ToonTanksCommon.h"

void UFireDamage::TriggerEffect_Implementation(AActor* DamagedActor, AController* Instigator)
{
	UToonTanksCommon::SetEffectOnActor(DamagedActor,
		FPrimaryAssetId(UToonTanksAssetManager::EffectType, "EffectBurning"),
		Instigator);
}
