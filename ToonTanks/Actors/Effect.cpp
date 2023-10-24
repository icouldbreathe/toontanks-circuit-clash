// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/BasePawn.h"

// Sets default values
AEffect::AEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
}

// Called when the game starts or when spawned
void AEffect::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<ABasePawn>(GetOwner());

	if (OwnerPawn)
	{
		OwnerPawn->OnPawnDied.AddDynamic(this, &AEffect::OnPawnDied);
	}

	GetOwner()->OnDestroyed.AddDynamic(this, &AEffect::OnOwnerDestroyed);

	StartEffect();
}

ABasePawn* AEffect::GetOwnerPawn() const
{
	return OwnerPawn;
}

void AEffect::OnOwnerDestroyed(AActor* ActorDestroyed)
{
	Destroy();
}

void AEffect::OnPawnDied(ABasePawn* DeadPawn)
{
	Destroy();
}

void AEffect::StartEffect()
{
	if (OnStartSideEffect.IsBound())
	{
		OnStartSideEffect.Broadcast();
	}

	UpdateLifetime();
}

void AEffect::RestartEffect()
{
	if (OnEndSideEffect.IsBound())
	{
		OnEndSideEffect.Broadcast();
	}

	if (OnStartSideEffect.IsBound())
	{
		OnStartSideEffect.Broadcast();
	}

	UpdateLifetime();
}

void AEffect::UpdateLifetime(const float Lifetime)
{
	GetWorldTimerManager().SetTimer(
		EffectTimerHandle,
		this,
		&AEffect::EndEffect,
		Lifetime == 0 ? EffectLifetime : Lifetime,
		false
		);
}

void AEffect::EndEffect()
{
	if (OnEndSideEffect.IsBound())
	{
		OnEndSideEffect.Broadcast();
	}

	Destroy();
}

AEffect* AEffect::FinishSpawning()
{
	return Cast<AEffect>(UGameplayStatics::FinishSpawningActor(this, GetOwner()->GetTransform()));
}
