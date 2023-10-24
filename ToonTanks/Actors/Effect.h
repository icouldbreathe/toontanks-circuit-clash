// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Effect.generated.h"

class ABasePawn;
enum class EParticleLifetimeRule : uint8;
class UNiagaraSystem;
class AParticle;
class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSideEffectSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSideEffectSignature);

UCLASS(Abstract)
class TOONTANKS_API AEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEffect();

	UFUNCTION(BlueprintCallable)
	AEffect* FinishSpawning();

	UPROPERTY(BlueprintAssignable)
	FOnStartSideEffectSignature OnStartSideEffect;
	UPROPERTY(BlueprintAssignable)
	FOnEndSideEffectSignature OnEndSideEffect;

	UFUNCTION(BlueprintCallable)
	void StartEffect();
	UFUNCTION(BlueprintCallable)
	void RestartEffect();
	UFUNCTION(BlueprintCallable)
	void UpdateLifetime(const float Lifetime = 0);
	UFUNCTION(BlueprintCallable)
	void EndEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	float EffectLifetime = 1.f;

	UFUNCTION(BlueprintCallable)
	ABasePawn* GetOwnerPawn() const;

private:
	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);
	UFUNCTION()
	void OnPawnDied(ABasePawn* DeadPawn);

	FTimerHandle EffectTimerHandle;

	UPROPERTY()
	ABasePawn* OwnerPawn;
};
