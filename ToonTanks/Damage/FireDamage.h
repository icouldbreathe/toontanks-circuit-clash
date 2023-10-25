// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "ToonTanks/Interfaces/DamageTriggerEffectInterface.h"
#include "FireDamage.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UFireDamage : public UDamageType, public IDamageTriggerEffectInterface
{
	GENERATED_BODY()

public:
	virtual void TriggerEffect_Implementation(AActor* DamagedActor, AController* Instigator) override;
};
