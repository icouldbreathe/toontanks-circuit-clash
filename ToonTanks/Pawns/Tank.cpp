// GPL-2.0 license

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToonTanks/ToonTanksPlayerController.h"
#include "ToonTanks/Components/GravityComponent.h"
#include "ToonTanks/Components/ProjectileInventory.h"
#include "ToonTanks/Data/InputConfigData.h"
#include "ToonTanks/Interfaces/GravityInterface.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TankPlayerController)
	{
		return;
	}

	RotateByCursor();
}

void ATank::HandleDestruction_Implementation()
{
	Super::HandleDestruction_Implementation();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetAlive(false);
}

AToonTanksPlayerController* ATank::GetTankPlayerController() const
{
	return TankPlayerController;
}

bool ATank::IsAlive() const
{
	return bAlive;
}

void ATank::SetAlive(bool bNewAlive)
{
	bAlive = bNewAlive;
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TankPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (!TankPlayerController)
	{
		return;
	}

	if (!(InputMapping && InputActions))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Failed to setup Player Input Component in %s, assign InputMapping and InputActions!"),
			*GetName()
			);
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			TankPlayerController->GetLocalPlayer()
			);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &ATank::Move);
	PEI->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &ATank::Turn);
	PEI->BindAction(InputActions->InputFire, ETriggerEvent::Triggered, this, &ATank::Fire);
	PEI->BindAction(InputActions->InputChangeProjectile, ETriggerEvent::Triggered, this,
		&ATank::ChangeProjectileOffset);
	PEI->BindAction(InputActions->InputSlot, ETriggerEvent::Triggered, this, &ATank::ChangeProjectileSlot);
}

void ATank::Fire()
{
	if (!bCanFire)
	{
		return;
	}

	Super::Fire();
}

/**
* Move Tank based on input value. Works on slopes by determining slope angle by tracing in the direction of movement.
* @param Value Input Action Value
*/
void ATank::Move(const FInputActionValue& Value)
{
	FHitResult ForwardHitResult;
	float SlopePitchAngle, SlopeRollAngle;

	// Rotate Tank based on slope angle returned by a trace hit
	if (ForwardTrace(Value.Get<float>(), ForwardHitResult))
	{
		UKismetMathLibrary::GetSlopeDegreeAngles(
			GetActorRightVector(),
			ForwardHitResult.Normal,
			FVector::UpVector,
			SlopePitchAngle,
			SlopeRollAngle);

		SlopeRollAngle = -SlopeRollAngle;

		FRotator NewRotation = FRotator(SlopePitchAngle, GetActorRotation().Yaw, SlopeRollAngle);
		FRotator DeltaRotation = FMath::RInterpTo(GetActorRotation(), NewRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this), 10.f);
		
		SetActorRotation(DeltaRotation);
	}

	FVector WorldOffset = GetActorForwardVector() * Value.Get<float>() * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed * PawnSpeed;

	// Add world offset split by components to allow sliding movement when either direction is blocking
	AddActorWorldOffset(FVector(WorldOffset.X, 0.f, 0.f), true);
	AddActorWorldOffset(FVector(0.f, WorldOffset.Y, 0.f), true);
	AddActorWorldOffset(FVector(0.f, 0.f, WorldOffset.Z), true);
}

void ATank::Turn(const FInputActionValue& Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value.Get<float>() * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate * PawnSpeed;
	AddActorLocalRotation(DeltaRotation, true);
}

/**
* Rotate turret by the position of the cursor in world space.
* In case the cursor falls on far away objects or no objects at all, it will take a predetermined distance to which the turret will turn to.
*/
void ATank::RotateByCursor() const
{
	if (TankPlayerController->IsLookInputIgnored())
	{
		return;
	}

	FVector CursorWorldPosition, CursorWorldDirection;
	TankPlayerController->DeprojectMousePositionToWorld(CursorWorldPosition, CursorWorldDirection);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CursorWorldPosition,
		CursorWorldPosition + CursorWorldDirection * CursorTraceDistanceOnHit,
		ECC_Visibility
		))
	{
		RotateTurret(HitResult.ImpactPoint);
	}
	else
	{
		RotateTurret(CursorWorldPosition + CursorWorldDirection * CursorTraceDistanceOnVoid);
	}
}

void ATank::ChangeProjectileOffset(const FInputActionValue& Value)
{
	ProjectileInventory->EquipSlotWithOffset(Value.Get<float>() >= 0.f ? 1 : -1);
}

void ATank::ChangeProjectileSlot(const FInputActionValue& Value)
{
	ProjectileInventory->EquipSlot(FMath::CeilToInt32(Value.Get<float>() - 1.f));
}

bool ATank::ForwardTrace(float Direction, FHitResult& Hit)
{
	FVector StartTrace =
		Direction * UKismetMathLibrary::GetForwardVector(GetActorRotation()) * 100.f + GetActorLocation();

	FVector EndTrace = StartTrace;
	EndTrace.Z -= 500.f;

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	
	return GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartTrace,
		EndTrace,
		ECC_WorldDynamic,
		CollisionParameters);
}

void ATank::AllowFire()
{
	bCanFire = true;
}
