// GPL-2.0 license


#include "GravityComponent.h"

#include "PlatformMovement.h"
#include "Kismet/GameplayStatics.h"
#include <ToonTanks/Interfaces/MovingGroundInterface.h>

// Sets default values for this component's properties
UGravityComponent::UGravityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityComponent::EnableGravity(bool bEnable)
{
	bGravityEnabled = bEnable;
}

// Called when the game starts
void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	CollisionParameters.AddIgnoredActor(GetOwner());
}

// Called every frame
void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bGravityEnabled)
	{
		return;
	}

	// Trace downwards to ground to determine whether owner should be falling
	FVector EndTrace = GetOwner()->GetActorLocation();
	EndTrace.Z -= GroundTraceOffset;

	FHitResult GroundHitResult;
	if (GetWorld()->LineTraceSingleByChannel(
		GroundHitResult,
		GetOwner()->GetActorLocation(),
		EndTrace,
		ECC_Visibility,
		CollisionParameters))
	{
		OnGround(GroundHitResult, DeltaTime);
	}
	else
	{
		OnFalling(DeltaTime);
	}
}

void UGravityComponent::OnFalling(const float& DeltaTime)
{
	// Return to normal rotation when falling
	FVector DeltaLocation = FVector::ZeroVector;
	const FRotator NewRotation = FRotator(0.f, GetOwner()->GetActorRotation().Yaw, 0.f);

	const FRotator DeltaRotation = FMath::RInterpTo(
		GetOwner()->GetActorRotation(),
		NewRotation,
		DeltaTime,
		10.f);

	GetOwner()->SetActorRotation(DeltaRotation);

	// Disable collision so that the owner doesn't get stuck on edges when falling
	// This will make the owner immune to attacks while falling.
	GetOwner()->SetActorEnableCollision(false);

	// Fake gravity
	if (GravityVelocity >= KillVelocity)
	{
		UGameplayStatics::ApplyDamage(GetOwner(), ToonTanksConstants::MAX_HEALTH, GetOwner()->GetInstigatorController(),
			GetOwner(),
			UDamageType::StaticClass());
	}
	else
	{
		GravityVelocity += 9.8f;
	}

	DeltaLocation.Z = -1.f * DeltaTime * GravityVelocity;
	GetOwner()->AddActorLocalOffset(DeltaLocation, true);
}

void UGravityComponent::OnGround(const FHitResult& GroundHitResult, const float& DeltaTime)
{
	FVector GroundOffset = GroundHitResult.ImpactPoint;
	GroundOffset.Z += DistanceToGroundAtRest;

	// Let owner move with the ground if ground is moving
	FVector GroundVelocity = FVector::ZeroVector;
	auto MovingComponent = GroundHitResult.GetActor()->FindComponentByInterface(UMovingGroundInterface::StaticClass());
	if (MovingComponent)
	{
		GroundVelocity = IMovingGroundInterface::Execute_GetMovementVelocity(MovingComponent);
		GroundVelocity.Z = 0.f;
	}

	GravityVelocity = 0.f;
	GetOwner()->SetActorEnableCollision(true);

	// Snap owner to ground
	GetOwner()->SetActorLocation(
		FVector(
			GetOwner()->GetActorLocation().X,
			GetOwner()->GetActorLocation().Y,
			GroundOffset.Z)
		+ GroundVelocity * DeltaTime);
}
