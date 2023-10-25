// GPL-2.0 license


#include "FireDamage.h"

#include "ToonTanks/ToonTanksAssetManager.h"
#include "ToonTanks/ToonTanksCommon.h"

void UFireDamage::TriggerEffect_Implementation(AActor* DamagedActor, AController* Instigator)
{
	UToonTanksCommon::SetEffectOnActor(DamagedActor,
		FPrimaryAssetId(UToonTanksAssetManager::EffectType, "EffectBurning"),
		Instigator);
}
