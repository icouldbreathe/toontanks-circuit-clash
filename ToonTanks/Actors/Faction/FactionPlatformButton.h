// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFactionActor.h"
#include "FactionPlatform.h"
#include "FactionPlatformButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonTriggered, bool, Pressed);

class UBoxComponent;
/**
 * 
 */
UCLASS()
class TOONTANKS_API AFactionPlatformButton : public AFactionPlatform
{
	GENERATED_BODY()

public:
	AFactionPlatformButton();

	UFUNCTION(BlueprintCallable)
	void SetActive(bool bNewActive);
	UFUNCTION(BlueprintCallable)
	bool GetActive() const;
	UFUNCTION(BlueprintCallable)
	void Trigger(bool bTriggered);

	UPROPERTY(BlueprintAssignable, Category="Button")
	FOnButtonTriggered OnButtonTriggered;

	virtual void ChangeFactionColor_Implementation(EFaction Faction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Button")
	UStaticMeshComponent* ButtonMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Button")
	UBoxComponent* TriggerComp;
	UPROPERTY(EditDefaultsOnly, Category = "Button")
	float PressedButtonOffset = 6.f;
	UPROPERTY(EditAnywhere, Category = "Button")
	bool bStartActivated = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button")
	bool bTriggerActive = true;

private:
	UFUNCTION()
	void OnOverlapping(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);
	UFUNCTION()
	void OnOverlappingEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	bool bButtonPressed = false;
	bool bActive = true;
};
