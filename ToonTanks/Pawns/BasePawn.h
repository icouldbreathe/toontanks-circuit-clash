// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ToonTanks/Interfaces/DestroyableInterface.h"
#include "ToonTanks/Interfaces/ProjectileInterface.h"
#include "BasePawn.generated.h"

class UDifficultyConfig;
class AToonTanksGameMode;
class AProjectile;
struct FInputActionValue;
class UProjectileInventory;
class UNiagaraSystem;
class AParticle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnDiedSignature, ABasePawn*, DeadPawn);

UCLASS()
class TOONTANKS_API ABasePawn : public APawn, public IDestroyableInterface, public IProjectileInterface
{
	GENERATED_BODY()

public:
	ABasePawn();

	virtual void BeginPlay() override;
	virtual void HandleDestruction_Implementation() override;
	virtual void AddProjectileToInventory_Implementation(const FPrimaryAssetId ProjectileId) override;

	UFUNCTION(BlueprintCallable)
	void SetPawnSpeed(float Speed);
	UFUNCTION(BlueprintCallable)
	void ResetPawnSpeed();

	UFUNCTION(BlueprintCallable, Category="Overlay")
	UMaterialInstanceDynamic* CreateDynamicOverlay(UMaterialInterface* OverlayMaterial, FName Tag);
	UFUNCTION(BlueprintCallable, Category = "Overlay")
	void DestroyDynamicOverlay(FName Tag);

	UPROPERTY(BlueprintAssignable, Category="Death")
	FOnPawnDiedSignature OnPawnDied;

protected:
	void RotateTurret(const FVector& LookAtTarget) const;
	virtual void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicLightsMat;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UNiagaraSystem* DeathParticlesNiagaraSystem;
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UProjectileInventory* ProjectileInventory;

	FTimerHandle FireTimerHandle;
	float FireRate = 2.f;
	float PawnSpeed;

private:
	UFUNCTION()
	void EquipProjectile(const TArray<FPrimaryAssetId>& Inventory, int32 SelectedSlotIndex, int32 PreviousSelectedSlotIndex);
	UPROPERTY()
	AParticle* DeathParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn", meta = (AllowPrivateAccess="true"))
	float BasePawnSpeed = 1.f;

	UPROPERTY()
	AToonTanksGameMode* GameMode;
};
