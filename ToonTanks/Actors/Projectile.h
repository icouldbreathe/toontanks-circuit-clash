// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UNiagaraSystem;
class AParticle;
class UProjectileMovementComponent;
class USoundBase;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Damage = 25.f;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UNiagaraSystem* HitParticlesNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UNiagaraSystem* TrailParticlesNiagaraSystem;

	UPROPERTY()
	AParticle* TrailParticles;
	UPROPERTY()
	AParticle* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Radial Damage")
	bool bRadialDamage = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Radial Damage")
	float DamageRadius = 100.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);
};
