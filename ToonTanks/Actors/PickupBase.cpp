// GPL-2.0 license


#include "PickupBase.h"

#include "NiagaraComponent.h"
#include "Particle.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/ToonTanksCommon.h"
#include "ToonTanks/Components/GravityComponent.h"
#include "ToonTanks/Pawns/Tank.h"

// Sets default values
APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	PickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	PickupTrigger->SetupAttachment(RootComponent);
	PickupTrigger->SetSphereRadius(150.f);

	GravityComponent = CreateDefaultSubobject<UGravityComponent>(TEXT("Gravity"));
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	if (PickupParticlesNiagaraSystem)
	{
		PickupParticles = UToonTanksCommon::SpawnParticleAttached(
			this,
			PickupParticlesNiagaraSystem,
			PickupMesh->GetComponentTransform(),
			PickupMesh,
			EParticleLifetimeRule::DiesOnOwnerDestroyed);

		if (PickupParticles)
		{
			PickupParticles->GetNiagaraComponent()->SetColorParameter("Color", ParticleColor);
		}
	}

	PickupMesh->SetGenerateOverlapEvents(true);
	PickupMesh->SetCollisionProfileName("OverlapAllDynamic", true);
	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::TriggerPickupSphere);
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimatePickup(DeltaTime);

	if (bMoveToTarget)
	{
		MoveToTarget(DeltaTime);
	}

	if (TargetActor && FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation()) < 25.f)
	{
		bMoveToTarget = false;
		PickupItem(TargetActor, PickupItemId);
		Destroy();
	}
}

void APickupBase::TriggerPickupSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& HitResult)
{
	ATank* Tank = Cast<ATank>(OtherActor);
	if (Tank && Tank->IsAlive())
	{
		TargetActor = Tank;
		GravityComponent->SetGravity(false);
		bMoveToTarget = true;
	}
}

void APickupBase::AnimatePickup(const float& DeltaTime) const
{
	PickupMesh->AddLocalRotation(RotationOffset);
	PickupMesh->AddLocalOffset(FVector(
		0.f,
		0.f,
		OscillationOffset * FMath::Sin(UGameplayStatics::GetTimeSeconds(this) * OscillationSpeed)
		));
}

void APickupBase::MoveToTarget(const float& DeltaTime)
{
	const FVector DeltaLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetActor->GetActorLocation(), DeltaTime,
		MoveToTargetSpeed);
	SetActorLocation(DeltaLocation, true);
}
