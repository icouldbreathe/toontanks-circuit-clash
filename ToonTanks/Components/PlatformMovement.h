// GPL-2.0 license

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToonTanks/ToonTanksTypes.h"
#include "ToonTanks/Interfaces/MovingGroundInterface.h"
#include "PlatformMovement.generated.h"

enum class EMoveDirection : uint8;
class AActuatorBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartMovingSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopMovingSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UPlatformMovement : public UActorComponent, public IMovingGroundInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlatformMovement();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetShouldMove(bool bInShouldMove, EMoveDirection InMoveDirection = EMoveDirection::Forward);

	virtual FVector GetMovementVelocity_Implementation() override;

	UPROPERTY(BlueprintAssignable, Category="Movement")
	FOnStartMovingSignature OnStartMoving;
	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnStopMovingSignature OnStopMoving;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	FVector LocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float Speed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	bool bOnLoop = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	bool bShouldMove = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	EMoveDirection MoveDirection = EMoveDirection::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	AActuatorBox* Actuator;

private:
	void Move(const float& DeltaTime);

	UFUNCTION()
	void OnTriggered(AActor* OverlappedActor, AActor* OtherActor);

	bool bIsMoving = false;
	FVector OriginalLocation;
	float InterpAlpha = 0.f;
};
