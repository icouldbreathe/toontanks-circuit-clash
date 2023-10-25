// GPL-2.0 license


#include "PlatformMovement.h"

#include "ToonTanks/Actors/ActuatorBox.h"

// Sets default values for this component's properties
UPlatformMovement::UPlatformMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlatformMovement::Move(const float& DeltaTime)
{
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector TargetLocation = OriginalLocation + LocationOffset;

	if (MoveDirection == EMoveDirection::Backward)
	{
		TargetLocation = OriginalLocation;
	}

	if (CurrentLocation != TargetLocation)
	{
		bIsMoving = true;
	}
	else
	{
		bIsMoving = false;
	}

	const FVector DeltaLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(DeltaLocation);

	if (!bOnLoop && !bIsMoving)
	{
		SetShouldMove(false);
	}

	if (bOnLoop && CurrentLocation == OriginalLocation + LocationOffset)
	{
		MoveDirection = EMoveDirection::Backward;
	}

	if (bOnLoop && CurrentLocation == OriginalLocation)
	{
		MoveDirection = EMoveDirection::Forward;
	}
}

void UPlatformMovement::OnTriggered(AActor* OverlappedActor, AActor* OtherActor)
{
	SetShouldMove(true, MoveDirection);
}

void UPlatformMovement::SetShouldMove(bool bInShouldMove, EMoveDirection InMoveDirection)
{
	bShouldMove = bInShouldMove;
	MoveDirection = InMoveDirection;

	if (bInShouldMove)
	{
		if (OnStartMoving.IsBound())
		{
			OnStartMoving.Broadcast();
		}
	}
	else
	{
		if (OnStopMoving.IsBound())
		{
			OnStopMoving.Broadcast();
		}
	}
}

FVector UPlatformMovement::GetMovementVelocity_Implementation()
{
	if (bIsMoving)
	{
		return LocationOffset.GetSafeNormal() * Speed * (MoveDirection == EMoveDirection::Backward ? -1.f : 1.f);
	}

	return FVector::ZeroVector;
}

// Called when the game starts
void UPlatformMovement::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();

	if (Actuator)
	{
		Actuator->OnTriggered.AddDynamic(this, &UPlatformMovement::OnTriggered);
	}
}


// Called every frame
void UPlatformMovement::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldMove)
	{
		Move(DeltaTime);
	}
}
