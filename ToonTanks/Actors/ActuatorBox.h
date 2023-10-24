// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ActuatorBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActuatorTriggeredSignature, AActor*, ActuatorActor, AActor*, OtherActor)
;

/**
 * 
 */
UCLASS()
class TOONTANKS_API AActuatorBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	AActuatorBox();

	UPROPERTY(BlueprintAssignable, Category="Actuator")
	FOnActuatorTriggeredSignature OnTriggered;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	bool bTriggerOnBeginOverlap = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	bool bTriggerOnEndOverlap = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	TArray<FName> TriggerTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	bool bTriggerOnlyByPlayer = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	int32 PlayerIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	bool bDestroyOnTrigger = true;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void Trigger(AActor* TriggeringActor);
	bool ContainsAnyTag(const AActor* Actor) const;
};
