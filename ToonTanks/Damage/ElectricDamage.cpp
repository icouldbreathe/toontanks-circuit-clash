// GPL-2.0 license


#include "ElectricDamage.h"

#include "ToonTanks/ToonTanksAssetManager.h"
#include "ToonTanks/ToonTanksCommon.h"

void UElectricDamage::TriggerEffect_Implementation(AActor* DamagedActor, AController* Instigator)
{
	UToonTanksCommon::SetEffectOnActor(DamagedActor,
		FPrimaryAssetId(UToonTanksAssetManager::EffectType, "EffectShocked"),
		Instigator);
}
