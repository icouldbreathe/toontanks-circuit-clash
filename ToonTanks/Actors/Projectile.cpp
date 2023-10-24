// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Particle.h"
#include "ToonTanks/ToonTanksCommon.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	ProjectileMovementComponent->InitialSpeed = 1300.f;
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (TrailParticlesNiagaraSystem)
	{
		TrailParticles = UToonTanksCommon::SpawnParticleAttached(
			this,
			TrailParticlesNiagaraSystem,
			GetActorTransform(),
			RootComponent,
			EParticleLifetimeRule::DetachesAndDeactivatesOnOwnerDestroyed);
	}

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation(), GetActorRotation());
	}

	SetLifeSpan(3.f);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		Destroy();
		return;
	}

	if (!DamageTypeClass)
	{
		DamageTypeClass = UDamageType::StaticClass();
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();

	// Other actor does not exist, is this actor or owner
	if (!OtherActor || OtherActor == this || OtherActor == MyOwner)
	{
		Destroy();
		return;
	}

	if (bRadialDamage)
	{
		UGameplayStatics::ApplyRadialDamage(
			this,
			Damage,
			Hit.Location,
			DamageRadius,
			DamageTypeClass,
			TArray({MyOwner}),
			this,
			MyOwnerInstigator);
	}
	else
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
	}

	if (HitParticlesNiagaraSystem)
	{
		HitParticles = UToonTanksCommon::SpawnParticle(this, HitParticlesNiagaraSystem, GetActorTransform());
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), GetActorRotation());
	}

	if (HitCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
	}

	Destroy();
}
