// GPL-2.0 license


#include "FactionLevelManager.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/ToonTanksGameMode.h"
#include "ToonTanks/Data/FactionConfigData.h"
#include "ToonTanks/Interfaces/FactionColorInterface.h"
#include "ToonTanks/Pawns/Tower.h"

// Sets default values
AFactionLevelManager::AFactionLevelManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFactionLevelManager::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));

	// Get saved state or use initial
	const EFaction CapturedState = GameMode->GetCapturedState(LevelID);
	if (CapturedState != EFaction::Faction_None)
	{
		OwningFaction = CapturedState;
	}

	// Delay in case actors not loaded
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DelayHandle,
		this,
		&AFactionLevelManager::UpdateLevelState,
		0.1f,
		false
		);
}

void AFactionLevelManager::OnTowerDied(ABasePawn* DeadTower)
{
	--TowerCount;

	if (TowerCount <= 0)
	{
		OwningFaction = EFaction::Faction_Tank;
		GameMode->LevelCaptured(LevelID, OwningFaction);
		UGameplayStatics::PlaySound2D(this, CaptureSound);
		UpdateLevelState();
	}
}

void AFactionLevelManager::UpdateLevelState()
{
	for (AActor* Actor : GetLevel()->Actors)
	{
		if (!Actor)
		{
			continue;
		}

		if (Actor->Implements<UFactionColorInterface>())
		{
			IFactionColorInterface::Execute_ChangeFactionColor(Actor, OwningFaction);
		}

		if (auto Tower = Cast<ATower>(Actor))
		{
			switch (OwningFaction)
			{
				case EFaction::Faction_Tank:
					Tower->Destroy();
					break;
				case EFaction::Faction_Tower:
					Tower->OnPawnDied.AddDynamic(this, &AFactionLevelManager::OnTowerDied);
					++TowerCount;
					break;
				case EFaction::Faction_None:
					break;
			}
		}
	}
}
