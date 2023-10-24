// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

class AToonTanksPlayerController;
/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction_Implementation() override;

	AToonTanksPlayerController* GetTankPlayerController() const;

	bool IsAlive() const;
	void SetAlive(bool bNewAlive);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cursor Rotation")
	float CursorTraceDistanceOnHit = 10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Rotation")
	float CursorTraceDistanceOnVoid = 2000.f;

	virtual void Fire() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 45.f;

	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void RotateByCursor() const;
	
	void ChangeProjectileOffset(const FInputActionValue& Value);
	void ChangeProjectileSlot(const FInputActionValue& Value);

	UPROPERTY()
	AToonTanksPlayerController* TankPlayerController;

	void AllowFire();
	bool bCanFire = true;
	bool bAlive = true;
};
