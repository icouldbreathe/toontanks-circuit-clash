// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "ToonTanks/Interfaces/DamageTriggerEffectInterface.h"
#include "IceDamage.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UIceDamage : public UDamageType, public IDamageTriggerEffectInterface
{
	GENERATED_BODY()

public:
	virtual void TriggerEffect_Implementation(AActor* DamagedActor, AController* Instigator) override;
};
