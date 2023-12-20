// GPL-2.0 license


#include "HealthComponent.h"

#include "StatusEffectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/ToonTanksGameMode.h"
#include "ToonTanks/ToonTanksTypes.h"
#include "ToonTanks/Data/DifficultyConfig.h"
#include "ToonTanks/Interfaces/DamageTriggerEffectInterface.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::SetHealth(float Value)
{
	if (Health <= 0.f)
	{
		return;
	}

	if (Value > ToonTanksConstants::MAX_HEALTH)
	{
		Value = ToonTanksConstants::MAX_HEALTH;
	}

	Health = FMath::Clamp(Value, 0.f, MaxHealth);

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(Health, MaxHealth);
	}
}

void UHealthComponent::SetMaxHealth(float Value)
{
	if (Value > ToonTanksConstants::MAX_HEALTH)
	{
		Value = ToonTanksConstants::MAX_HEALTH;
	}

	MaxHealth = Value;
}

void UHealthComponent::AddHealth(float Amount)
{
	SetHealth(Health + Amount);
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::SetInvincible(bool bEnable)
{
	bInvincible = bEnable;
}

void UHealthComponent::AddToHealth_Implementation(float Amount)
{
	AddHealth(Amount);
}

void UHealthComponent::GetActorHealth_Implementation(float& OutHealth, float& OutMaxHealth)
{
	OutHealth = Health;
	OutMaxHealth = MaxHealth;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCauser)
{
	if (bInvincible)
	{
		return;
	}
	if (Damage <= 0.f)
	{
		return;
	}
	if (Health <= 0.f)
	{
		return;
	}

	const UStatusEffectComponent* StatusEffectComp = Cast<UStatusEffectComponent>(
		GetOwner()->GetComponentByClass(UStatusEffectComponent::StaticClass())
		);

	if (StatusEffectComp && DamageType->Implements<UDamageTriggerEffectInterface>())
	{
		IDamageTriggerEffectInterface::Execute_TriggerEffect(DamageType->GetOutermostObject(), GetOwner(), Instigator);
	}

	float FinalDamage = Damage;

	if (ToonTanksGameMode->GetGameDifficulty())
	{
		FinalDamage = ToonTanksGameMode->GetGameDifficulty()->GetFinalDamage(
			Damage, DamagedActor, Instigator, DamageType);
	}

	AddHealth(FMath::CeilToFloat(-FinalDamage));

	if (Health <= 0.f && ToonTanksGameMode)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);
	}
}
