// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToonTanks/Interfaces/EffectInterface.h"
#include "StatusEffectComponent.generated.h"

USTRUCT()
struct FStatusEffect
{
	GENERATED_USTRUCT_BODY()
	;

	FStatusEffect()
	{
	}

	FStatusEffect(const FPrimaryAssetId& InEffectId, AEffect* InEffectActor)
		: EffectId(InEffectId)
		, EffectActor(InEffectActor)
	{
	}

	UPROPERTY()
	FPrimaryAssetId EffectId;
	UPROPERTY()
	AEffect* EffectActor = nullptr;

	bool operator==(const AActor* Other) const;
	bool operator==(const FPrimaryAssetId& Other) const;
};

class AEffect;
class UEffectData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UStatusEffectComponent : public UActorComponent, public IEffectInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float EffectRadius = 25.f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual AEffect* TriggerEffect_Implementation(const FPrimaryAssetId& EffectId, const AController* Instigator,
		bool bDeferred) override;

private:
	TArray<FStatusEffect> StatusEffects;

	bool IsAssetValid(const FPrimaryAssetId& AssetId) const;
	bool ValidateEffect(const FPrimaryAssetId& EffectId, const UEffectData& EffectData);
	AEffect* SpawnEffectDeferred(TSubclassOf<AEffect> EffectClass) const;
	void SetupEffect(AEffect* EffectActor, const AController* Instigator);
	AEffect* FinishSpawningEffect(AEffect* EffectActor) const;

	UFUNCTION()
	void OnEffectDestroyed(AActor* EffectActorDestroyed);
};
