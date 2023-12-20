// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToonTanks/Interfaces/GravityInterface.h"
#include "GravityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UGravityComponent : public UActorComponent, public IGravityInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGravityComponent();

	UFUNCTION(BlueprintCallable)
	void SetGravity(bool bEnable);

	virtual void EnableGravity_Implementation(bool bEnabled) override;
	virtual bool IsFalling_Implementation() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Gravity")
	float GroundTraceOffset = 100.f;
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float DistanceToGroundAtRest = 80.f;
	UPROPERTY(EditAnywhere, Category="Gravity")
	float KillVelocity = 2560.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	bool bGravityEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	float Acceleration = 980.f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OnFalling(const float& DeltaTime);
	void OnGround(const FHitResult& GroundHitResult, const float& DeltaTime);

	FCollisionQueryParams CollisionParameters;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity", meta=(AllowPrivateAccess = "true"))
	float GravityVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gravity", meta=(AllowPrivateAccess = "true"))
	bool bIsFalling = false;
};
