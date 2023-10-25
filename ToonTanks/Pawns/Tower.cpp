// GPL-2.0 license


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Actors/PickupBase.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&ATower::CheckFireCondition,
		FireRate,
		true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
		TurretMesh->SetRelativeLocation(
			FMath::VInterpConstantTo(
				TurretMesh->GetRelativeLocation(),
				FVector::ZeroVector,
				DeltaTime, 25.f * PawnSpeed
				),
			false
			);
	}
	else
	{
		RotateTurret(GetActorLocation() + GetActorForwardVector());
		TurretMesh->SetRelativeLocation(
			FMath::VInterpConstantTo(
				TurretMesh->GetRelativeLocation(),
				FVector(0.f, 0.f, -15.f),
				DeltaTime, 25.f * PawnSpeed
				),
			false
			);
	}
}

void ATower::HandleDestruction_Implementation()
{
	Super::HandleDestruction_Implementation();

	Destroy();
}

TArray<TSubclassOf<APickupBase>> ATower::GetItemsToDrop_Implementation()
{
	return DropItems;
}

void ATower::Activate(bool bNewActive)
{
	// On Activate
	if (!bActive && bNewActive)
	{
		DynamicLightsMat->SetScalarParameterValue("EmissiveStrength", 100.f);

		UGameplayStatics::PlaySoundAtLocation(
			this,
			ActivationSound,
			GetActorLocation()
			);
	}

	// On Deactivate
	if (bActive && !bNewActive)
	{
		DynamicLightsMat->SetScalarParameterValue("EmissiveStrength", 0.f);
	}

	bActive = bNewActive;
}

void ATower::CheckFireCondition()
{
	if (!Tank)
	{
		return;
	}
	if (InFireRange() && IsTargetVisible() && Tank->IsAlive())
	{
		Fire();
	}
}

bool ATower::InFireRange()
{
	if (!Tank)
	{
		return false;
	}

	const float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
	if (Distance <= FireRange)
	{
		Activate(true);
		return true;
	}

	Activate(false);
	return false;
}

bool ATower::IsTargetVisible() const
{
	if (!FireOnSightOnly)
	{
		return true;
	}

	if (!Tank)
	{
		return false;
	}

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetActorLocation(),
		Tank->GetActorLocation(),
		ECC_Visibility,
		CollisionParameters))
	{
		if (HitResult.GetActor() == Tank)
		{
			return true;
		}
	}
	return false;
}
