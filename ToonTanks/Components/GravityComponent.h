// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UGravityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGravityComponent();

	UFUNCTION(BlueprintCallable)
	void EnableGravity(bool bEnable);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Gravity")
	float GroundTraceOffset = 100.f;
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float DistanceToGroundAtRest = 80.f;
	UPROPERTY(EditAnywhere, Category="Gravity")
	float KillVelocity = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	bool bGravityEnabled = true;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OnFalling(const float& DeltaTime);
	void OnGround(const FHitResult& GroundHitResult, const float& DeltaTime);

	FCollisionQueryParams CollisionParameters;
	float GravityVelocity = 0.f;
};
