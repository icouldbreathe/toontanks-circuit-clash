// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;
class UNiagaraSystem;
class AParticle;
class UBoxComponent;
class UGravityComponent;

UCLASS(Abstract)
class TOONTANKS_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* PickupMesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* PickupTrigger;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UGravityComponent* GravityComponent;

	UPROPERTY()
	AParticle* PickupParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Particles")
	UNiagaraSystem* PickupParticlesNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Particles")
	FLinearColor ParticleColor = FLinearColor(FColor::White);

	UPROPERTY(BlueprintReadOnly)
	AActor* TargetActor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pickup Item")
	FPrimaryAssetId PickupItemId;

	UFUNCTION(BlueprintImplementableEvent)
	void PickupItem(AActor* PickingActor, FPrimaryAssetId ItemId);

	UPROPERTY(EditAnywhere, Category="Pickup Movement")
	FRotator RotationOffset = FRotator(0.f, 3.f, 0.f);
	UPROPERTY(EditAnywhere, Category="Pickup Movement")
	float OscillationOffset = 0.8f;
	UPROPERTY(EditAnywhere, Category="Pickup Movement")
	float OscillationSpeed = 2.f;
	UPROPERTY(EditAnywhere, Category="Pickup Movement")
	float MoveToTargetSpeed = 1200.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void TriggerPickupSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& HitResult);

	void AnimatePickup(const float& DeltaTime) const;
	void MoveToTarget(const float& DeltaTime);

	bool bMoveToTarget = false;
};
