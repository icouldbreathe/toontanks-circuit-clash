// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToonTanks/Data/FactionConfigData.h"
#include "FactionLevelManager.generated.h"

class APlayerStart;
class AToonTanksGameMode;
class ABasePawn;

UCLASS()
class TOONTANKS_API AFactionLevelManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFactionLevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Faction")
	EFaction OwningFaction = EFaction::Faction_Tower;

	/** ID of this level, in order of game progression. Starts at zero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	int32 LevelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* CaptureSound;

private:
	int32 TowerCount = 0;
	UPROPERTY()
	AToonTanksGameMode* GameMode;

	UFUNCTION()
	void OnTowerDied(ABasePawn* DeadTower);
	void UpdateLevelState();
};
