// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToonTanks/Data/FactionConfigData.h"
#include "ToonTanks/Interfaces/FactionColorInterface.h"
#include "BaseFactionActor.generated.h"

UCLASS()
class TOONTANKS_API ABaseFactionActor : public AActor, public IFactionColorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseFactionActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ChangeFactionColor_Implementation(EFaction Faction) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* FactionMeshComp;
	UPROPERTY(EditAnywhere, Category="Faction")
	EFaction OwningFaction;
	UPROPERTY()
	UFactionConfigData* FactionData;
};
