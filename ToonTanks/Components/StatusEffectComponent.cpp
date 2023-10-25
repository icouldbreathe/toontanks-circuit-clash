// GPL-2.0 license


#include "StatusEffectComponent.h"
#include "Engine/AssetManager.h"
#include "ToonTanks/ToonTanksAssetManager.h"
#include "ToonTanks/Actors/Effect.h"
#include "ToonTanks/Data/EffectData.h"
#include "Kismet/GameplayStatics.h"


bool FStatusEffect::operator==(const AActor* Other) const
{
	return EffectActor == Cast<AEffect>(Other);
}

bool FStatusEffect::operator==(const FPrimaryAssetId& Other) const
{
	return EffectId == Other;
}

// Sets default values for this component's properties
UStatusEffectComponent::UStatusEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AEffect* UStatusEffectComponent::TriggerEffect_Implementation(const FPrimaryAssetId& EffectId,
	const AController* Instigator, bool bDeferred)
{
	if (!IsAssetValid(EffectId))
	{
		return nullptr;
	}

	const UEffectData* EffectData = Cast<UEffectData>(UToonTanksAssetManager::Get().GetPrimaryAssetObject(EffectId));
	const TSubclassOf<AEffect> EffectClass = EffectData->EffectActor;

	if (!ValidateEffect(EffectId, *EffectData))
	{
		return nullptr;
	}

	AEffect* DeferredEffect = SpawnEffectDeferred(EffectClass);

	SetupEffect(DeferredEffect, Instigator);

	StatusEffects.Add(FStatusEffect(EffectId, DeferredEffect));

	if (!bDeferred)
	{
		AEffect* SpawnedEffect = FinishSpawningEffect(DeferredEffect);
		return SpawnedEffect;
	}

	return DeferredEffect;
}

bool UStatusEffectComponent::IsAssetValid(const FPrimaryAssetId& AssetId) const
{
	if (!UToonTanksAssetManager::IsPrimaryAssetIdValid(AssetId) || AssetId.PrimaryAssetType !=
	    UToonTanksAssetManager::EffectType)
	{
		UE_LOG(LogTemp, Error, TEXT("Non valid asset ID passed to effect."));
		return false;
	}

	return true;
}

bool UStatusEffectComponent::ValidateEffect(const FPrimaryAssetId& EffectId, const UEffectData& EffectData)
{
	// Find if the same effect exists
	if (!EffectData.AllowMultiple && StatusEffects.Contains(EffectId))
	{
		StatusEffects.FindByKey(EffectId)->EffectActor->RestartEffect();
		return false;
	}

	// Find if negative effect exists
	if (!EffectData.NeutralizeEffects.IsEmpty())
	{
		for (FPrimaryAssetId Effect : EffectData.NeutralizeEffects)
		{
			if (StatusEffects.Contains(Effect))
			{
				StatusEffects.FindByKey(Effect)->EffectActor->EndEffect();
			}
		}
	}

	return true;
}

AEffect* UStatusEffectComponent::SpawnEffectDeferred(TSubclassOf<AEffect> EffectClass) const
{
	AEffect* DeferredEffect = Cast<AEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
		this,
		EffectClass,
		GetOwner()->GetTransform(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		GetOwner()
		));

	check(DeferredEffect);

	return DeferredEffect;
}

void UStatusEffectComponent::SetupEffect(AEffect* EffectActor, const AController* Instigator)
{
	if (Instigator)
	{
		EffectActor->SetInstigator(Instigator->GetPawn());
	}

	EffectActor->AttachToActor(
		GetOwner(),
		FAttachmentTransformRules::SnapToTargetIncludingScale
		);

	EffectActor->OnDestroyed.AddDynamic(this, &UStatusEffectComponent::OnEffectDestroyed);
}

AEffect* UStatusEffectComponent::FinishSpawningEffect(AEffect* EffectActor) const
{
	check(EffectActor);

	if (AEffect* SpawnedEffect = Cast<AEffect>(
		UGameplayStatics::FinishSpawningActor(EffectActor, GetOwner()->GetTransform())))
	{
		return SpawnedEffect;
	}

	return nullptr;
}

void UStatusEffectComponent::OnEffectDestroyed(AActor* EffectActorDestroyed)
{
	const int32 Index = StatusEffects.IndexOfByKey(EffectActorDestroyed);
	StatusEffects.RemoveAt(Index);
}
