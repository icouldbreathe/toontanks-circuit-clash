// GPL-2.0 license


#include "BasePawn.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/ToonTanksCommon.h"
#include "ToonTanks/Components/ProjectileInventory.h"
#include "ToonTanks/Data/Items/ProjectileItem.h"


// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	ProjectileInventory = CreateDefaultSubobject<UProjectileInventory>(TEXT("Projectile Inventory"));

	PawnSpeed = BasePawnSpeed;
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	ProjectileInventory->OnSelectedProjectileSlotChange.AddDynamic(this, &ABasePawn::EquipProjectile);

	DynamicLightsMat = UMaterialInstanceDynamic::Create(TurretMesh->GetMaterial(1), this);

	BaseMesh->SetMaterial(1, DynamicLightsMat);
	TurretMesh->SetMaterial(1, DynamicLightsMat);

	EquipProjectile(ProjectileInventory->GetInventory(), ProjectileInventory->GetEquippedSlotIndex(), INDEX_NONE);
}

void ABasePawn::HandleDestruction_Implementation()
{
	if (OnPawnDied.IsBound())
	{
		OnPawnDied.Broadcast(this);
	}

	if (DeathParticlesNiagaraSystem)
	{
		DeathParticles = UToonTanksCommon::SpawnParticle(this, DeathParticlesNiagaraSystem, GetActorTransform());
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	}
	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

void ABasePawn::AddProjectileToInventory_Implementation(const FPrimaryAssetId ProjectileId)
{
	const int32 Index = ProjectileInventory->AddProjectile(ProjectileId);
	if (Index != INDEX_NONE)
	{
		ProjectileInventory->EquipSlot(Index);
	}
}

UMaterialInstanceDynamic* ABasePawn::CreateDynamicOverlay(UMaterialInterface* OverlayMaterial, FName Tag)
{
	UStaticMeshComponent* BaseMeshCopy = Cast<UStaticMeshComponent>(
		AddComponentByClass(UStaticMeshComponent::StaticClass(), false, BaseMesh->GetRelativeTransform(), false));
	UStaticMeshComponent* TurretMeshCopy = Cast<UStaticMeshComponent>(
		AddComponentByClass(UStaticMeshComponent::StaticClass(), false, TurretMesh->GetRelativeTransform(), false));

	BaseMeshCopy->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TurretMeshCopy->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BaseMeshCopy->AttachToComponent(BaseMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	TurretMeshCopy->AttachToComponent(TurretMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);

	BaseMeshCopy->SetStaticMesh(BaseMesh->GetStaticMesh());
	TurretMeshCopy->SetStaticMesh(TurretMesh->GetStaticMesh());

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(OverlayMaterial, this);

	BaseMeshCopy->SetMaterial(0, DynamicMaterial);
	TurretMeshCopy->SetMaterial(0, DynamicMaterial);

	BaseMeshCopy->ComponentTags.Add(Tag);
	TurretMeshCopy->ComponentTags.Add(Tag);

	return DynamicMaterial;
}

void ABasePawn::DestroyDynamicOverlay(FName Tag)
{
	auto Meshes = GetComponentsByTag(UStaticMeshComponent::StaticClass(), Tag);

	for (const auto Mesh : Meshes)
	{
		Mesh->DestroyComponent();
	}
}

void ABasePawn::SetPawnSpeed(float Speed)
{
	PawnSpeed = Speed;
}

void ABasePawn::ResetPawnSpeed()
{
	PawnSpeed = BasePawnSpeed;
}

void ABasePawn::RotateTurret(const FVector& LookAtTarget) const
{
	const FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	const FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			5.f * PawnSpeed));
}

void ABasePawn::Fire()
{
	if (!ProjectileInventory->GetEquippedProjectileItem())
	{
		return;
	}

	const FTransform SpawnTransform = ProjectileSpawnPoint->GetComponentTransform();

	AActor* Projectile = GetWorld()->SpawnActor(
		ProjectileInventory->GetEquippedProjectileItem()->ItemActor,
		&SpawnTransform
		);

	if (Projectile)
	{
		Projectile->SetOwner(this);
	}
}

void ABasePawn::EquipProjectile(const TArray<FPrimaryAssetId>& Inventory, int32 SelectedSlotIndex, int32 PreviousSelectedSlotIndex)
{
	UProjectileItem* Projectile = ProjectileInventory->GetEquippedProjectileItem();
	if (Projectile)
	{
		FireRate = Projectile->FireRate;

		if (DynamicLightsMat)
		{
			DynamicLightsMat->SetVectorParameterValue("Color", Projectile->ItemColor);
		}
	}
}
