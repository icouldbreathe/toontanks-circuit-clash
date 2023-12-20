// GPL-2.0 license


#include "ToonTanksCommon.h"

#include "Actors/Particle.h"
#include "Interfaces/EffectInterface.h"
#include "Interfaces/HealthInterface.h"
#include "Interfaces/ProjectileInterface.h"
#include "Kismet/GameplayStatics.h"
#include "RHI.h"

AEffect* UToonTanksCommon::SetEffectOnActor(AActor* Actor, FPrimaryAssetId EffectId, AController* Instigator,
	bool bDeferred)
{
	auto EffectComponent = Actor->FindComponentByInterface<UEffectInterface>();
	if (EffectComponent)
	{
		return IEffectInterface::Execute_TriggerEffect(EffectComponent, EffectId, Instigator, bDeferred);
	}

	return nullptr;
}

void UToonTanksCommon::AddProjectileToActor(AActor* Actor, FPrimaryAssetId ProjectileId)
{
	if (Actor->Implements<UProjectileInterface>())
	{
		IProjectileInterface::Execute_AddProjectileToInventory(Actor, ProjectileId);
	}
}

void UToonTanksCommon::AddHealthToActor(AActor* Actor, float Amount)
{
	auto HealthComponent = Actor->FindComponentByInterface<UHealthInterface>();
	if (HealthComponent)
	{
		IHealthInterface::Execute_AddToHealth(HealthComponent, Amount);
	}
}

/**
 * Spawns particle in world. Particle dies when NiagaraSystem completes by default.
 */
AParticle* UToonTanksCommon::SpawnParticle(UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem,
	const FTransform& AbsoluteTransform)
{
	AParticle* Particle = Cast<AParticle>(
		WorldContextObject->GetWorld()->SpawnActor(AParticle::StaticClass(), &AbsoluteTransform,
			FActorSpawnParameters()));

	if (Particle)
	{
		Particle->SetNiagaraSystem(NiagaraSystem);
	}

	return Particle;
}

/**
 * Spawns particle attached to component. Particle dies when NiagaraSystem completes by default.
 * @param WorldContextObject
 * @param NiagaraSystem
 * @param SpawnTransform
 * @param ParentComponent Component to attach to.
 * @param InRule Rule to determine behaviour on owner destroyed. The particle gets destroyed by default.
 * @return 
 */
AParticle* UToonTanksCommon::SpawnParticleAttached(UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem,
	const FTransform& SpawnTransform,
	USceneComponent* ParentComponent, EParticleLifetimeRule InRule)
{
	if (!ParentComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not attach particle to null scene component."))
		return nullptr;
	}

	AParticle* DeferredParticle = Cast<AParticle>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
		WorldContextObject->GetWorld(),
		AParticle::StaticClass(),
		SpawnTransform,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		ParentComponent->GetOwner()));

	if (DeferredParticle)
	{
		DeferredParticle->SetNiagaraSystem(NiagaraSystem);
		DeferredParticle->SetLifetimeRule(InRule);
		DeferredParticle->Attach(ParentComponent);
	}

	UGameplayStatics::FinishSpawningActor(DeferredParticle, SpawnTransform);

	return DeferredParticle;
}

bool UToonTanksCommon::IsEditor()
{
	return GEngine->IsEditor();
}

FString UToonTanksCommon::GetAppVersion()
{
	FString AppVersion;

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni);

	return AppVersion;
}

FString UToonTanksCommon::GetRHIName()
{
	FString RHI = GDynamicRHI->GetName();

	if (IsRHIDeviceIntel())
	{
		RHI += TEXT("_INTEL");
	}
	else if (IsRHIDeviceAMD())
	{
		RHI += TEXT("_AMD");
	}
	else if (IsRHIDeviceNVIDIA())
	{
		RHI += TEXT("_NVIDIA");
	}

	return RHI;
}

/**
 * 
 * @param VersionA Version A
 * @param VersionB Version B
 * @return Bigger (Newer) version. Returns Version A if equal.
 */
FString UToonTanksCommon::GetBiggerVersion(const FString& VersionA, const FString& VersionB)
{
	FString VersionAFirst;
	FString VersionASecond;
	FString VersionAThird;

	VersionA.Split(TEXT("."), &VersionAFirst, &VersionASecond);
	VersionASecond.Split(TEXT("."), &VersionASecond, &VersionAThird);

	FString VersionBFirst;
	FString VersionBSecond;
	FString VersionBThird;

	VersionB.Split(TEXT("."), &VersionBFirst, &VersionBSecond);
	VersionBSecond.Split(TEXT("."), &VersionBSecond, &VersionBThird);

	if (FCString::Atoi(*VersionAFirst) < FCString::Atoi(*VersionBFirst))
	{
		return VersionB;
	}
	if (FCString::Atoi(*VersionAFirst) > FCString::Atoi(*VersionBFirst))
	{
		return VersionA;
	}

	if (FCString::Atoi(*VersionASecond) < FCString::Atoi(*VersionBSecond))
	{
		return VersionB;
	}
	if (FCString::Atoi(*VersionASecond) > FCString::Atoi(*VersionBSecond))
	{
		return VersionA;
	}

	if (FCString::Atoi(*VersionAThird) < FCString::Atoi(*VersionBThird))
	{
		return VersionB;
	}
	if (FCString::Atoi(*VersionAThird) > FCString::Atoi(*VersionBThird))
	{
		return VersionA;
	}

	return VersionA;
}
