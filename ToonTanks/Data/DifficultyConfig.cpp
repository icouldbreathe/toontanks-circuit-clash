// Fill out your copyright notice in the Description page of Project Settings.


#include "DifficultyConfig.h"

#include "ToonTanks/Pawns/Tank.h"
#include "ToonTanks/Pawns/Tower.h"

float UDifficultyConfig::GetFinalDamage(float Damage, AActor* DamagedActor, AController* Instigator,
	const UDamageType* DamageType) const
{
	float FinalDamage = Damage;

	if (Cast<ATank>(DamagedActor))
	{
		FinalDamage = Damage * TankReceiveDamageMultiplier;
	}

	if (Cast<ATower>(DamagedActor))
	{
		FinalDamage = Damage * TowerReceiveDamageMultiplier;
	}

	return FinalDamage;
}
