// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToonTanks/ToonTanksTypes.h"
#include "Particle.generated.h"

enum class EParticleLifetimeRule : uint8;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class TOONTANKS_API AParticle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AParticle();

	UFUNCTION(BlueprintCallable)
	void SetNiagaraSystem(UNiagaraSystem* NiagaraSystem);
	UFUNCTION(BlueprintCallable)
	void SetLifetimeRule(EParticleLifetimeRule InRule);
	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetNiagaraComponent() const;

	UFUNCTION(BlueprintCallable)
	void Attach(USceneComponent* SceneComponent, FName SocketName = NAME_None);
	UFUNCTION(BlueprintCallable)
	void Detach();

	UFUNCTION(BlueprintCallable)
	void Kill() const;
	UFUNCTION(BlueprintCallable)
	void Restart();
	UFUNCTION(BlueprintCallable)
	void Complete();
	UFUNCTION(BlueprintCallable)
	void DetachAndComplete();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	UNiagaraComponent* NiagaraComp;

	EParticleLifetimeRule LifetimeRule = EParticleLifetimeRule::DiesOnOwnerDestroyed;

	UFUNCTION()
	void ParticleSystemFinished(UNiagaraComponent* NiagaraComponent);

	UFUNCTION()
	void OnParticleOwnerDestroyed(AActor* ActorDestroyed);
};
