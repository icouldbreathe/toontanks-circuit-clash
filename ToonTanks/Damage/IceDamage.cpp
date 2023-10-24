// Fill out your copyright notice in the Description page of Project Settings.


#include "IceDamage.h"

#include "ToonTanks/ToonTanksAssetManager.h"
#include "ToonTanks/ToonTanksCommon.h"

void UIceDamage::TriggerEffect_Implementation(AActor* DamagedActor, AController* Instigator)
{
	UToonTanksCommon::SetEffectOnActor(DamagedActor,
		FPrimaryAssetId(UToonTanksAssetManager::EffectType, "EffectFrozen"), Instigator);
}
