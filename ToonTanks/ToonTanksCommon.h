// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Actors/Particle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToonTanksCommon.generated.h"

class AEffect;
enum class EParticleLifetimeRule : uint8;
class AParticle;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class TOONTANKS_API UToonTanksCommon : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Effect")
	static AEffect* SetEffectOnActor(AActor* Actor, FPrimaryAssetId EffectId, AController* Instigator = nullptr,
		bool bDeferred = false);
	UFUNCTION(BlueprintCallable, Category="Projectile")
	static void AddProjectileToActor(AActor* Actor, FPrimaryAssetId ProjectileId);
	UFUNCTION(BlueprintCallable, Category="Health")
	static void AddHealthToActor(AActor* Actor, float Amount);
	UFUNCTION(BlueprintCallable, Category="Particle", meta = (WorldContext = "WorldContextObject"))
	static AParticle* SpawnParticle(UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem,
		const FTransform& AbsoluteTransform);
	UFUNCTION(BlueprintCallable, Category="Particle", meta = (WorldContext = "WorldContextObject"))
	static AParticle* SpawnParticleAttached(UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem,
		const FTransform& SpawnTransform, USceneComponent* ParentComponent,
		EParticleLifetimeRule InRule = EParticleLifetimeRule::DiesOnOwnerDestroyed);
	UFUNCTION(BlueprintCallable, Category="Development")
	static bool IsEditor();

	UFUNCTION(BlueprintCallable, Category="App")
	static FString GetAppVersion();
	UFUNCTION(BlueprintCallable, Category="App")
	static FString GetRHIName();
	static FString GetBiggerVersion(const FString& A, const FString& B);
};
