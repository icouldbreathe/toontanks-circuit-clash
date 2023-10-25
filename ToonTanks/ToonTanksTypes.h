// GPL-2.0 license

#pragma once

#include "ToonTanksTypes.generated.h"

#define PRINT(text, time) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, time, FColor::White, text)
#define DEBUG(text, ...) if (GEngine->IsEditor()) UE_LOG(LogTemp, Warning, TEXT("%s: "##text), *GetFName().ToString(), __VA_ARGS__)

namespace ToonTanksConstants
{
constexpr float MAX_HEALTH = 999999.f;
}

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECornerFlags : uint8
{
	None = 0 UMETA(Hidden),
	Corner0 = 1 << 0,
	Corner1 = 1 << 1,
	Corner2 = 1 << 2,
	Corner3 = 1 << 3
};

ENUM_CLASS_FLAGS(ECornerFlags);

/**
 * Particle lifetime rule for determining behavior when owner gets destroyed.
 */
UENUM()
enum class EParticleLifetimeRule : uint8
{
	/** Immediately kills the particle not giving it time to complete. */
	DiesOnOwnerDestroyed,
	/** Detaches from owner and deactivates the particle giving it time to complete. */
	DetachesAndDeactivatesOnOwnerDestroyed,
	/** Detaches from owner and continues being active indefinitely or until it completes on its own. */
	DetachesAndContinuesOnOwnerDestroyed
};

UENUM()
enum class EMoveDirection : uint8
{
	None UMETA(Hidden),
	Forward,
	Backward
};
