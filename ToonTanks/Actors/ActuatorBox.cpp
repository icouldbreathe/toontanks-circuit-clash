// Fill out your copyright notice in the Description page of Project Settings.


#include "ActuatorBox.h"
#include "Kismet/GameplayStatics.h"

AActuatorBox::AActuatorBox()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AActuatorBox::BeginPlay()
{
	Super::BeginPlay();

	if (bTriggerOnBeginOverlap)
	{
		OnActorBeginOverlap.AddDynamic(this, &AActuatorBox::OnBeginOverlap);
	}

	if (bTriggerOnEndOverlap)
	{
		OnActorEndOverlap.AddDynamic(this, &AActuatorBox::OnEndOverlap);
	}
}

void AActuatorBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActuatorBox::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Trigger(OtherActor);
}

void AActuatorBox::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Trigger(OtherActor);
}

void AActuatorBox::Trigger(AActor* TriggeringActor)
{
	if (bTriggerOnlyByPlayer && TriggeringActor != UGameplayStatics::GetPlayerPawn(this, PlayerIndex))
	{
		return;
	}

	if (!TriggerTags.IsEmpty() && !ContainsAnyTag(TriggeringActor))
	{
		return;
	}

	if (OnTriggered.IsBound())
	{
		OnTriggered.Broadcast(this, TriggeringActor);

		if (bDestroyOnTrigger)
		{
			Destroy();
		}
	}
}

bool AActuatorBox::ContainsAnyTag(const AActor* Actor) const
{
	if (TriggerTags.IsEmpty())
	{
		return false;
	}

	for (const FName Tag : TriggerTags)
	{
		if (Actor->ActorHasTag(Tag))
		{
			return true;
		}
	}

	return false;
}
