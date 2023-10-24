// Fill out your copyright notice in the Description page of Project Settings.


#include "FactionPlatformButton.h"

#include "Components/BoxComponent.h"
#include "ToonTanks/Pawns/BasePawn.h"

AFactionPlatformButton::AFactionPlatformButton()
{
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	ButtonMesh->SetupAttachment(FactionMeshComp);

	TriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	TriggerComp->SetupAttachment(FactionMeshComp);
}

void AFactionPlatformButton::ChangeFactionColor_Implementation(EFaction Faction)
{
	Super::ChangeFactionColor_Implementation(Faction);

	// CPD Index 4: Alpha
	if (bButtonPressed)
	{
		FactionMeshComp->SetCustomPrimitiveDataFloat(4, 1.f);
		ButtonMesh->SetCustomPrimitiveDataFloat(4, 1.f);
	}
	else
	{
		FactionMeshComp->SetCustomPrimitiveDataFloat(4, 0.f);

		if (!bActive)
		{
			ButtonMesh->SetCustomPrimitiveDataFloat(4, 0.f);
		}
	}
}

void AFactionPlatformButton::SetActive(bool bNewActive)
{
	bActive = bNewActive;

	if (bActive)
	{
		FactionMeshComp->SetCustomPrimitiveDataFloat(4, 0.f);
		ButtonMesh->SetCustomPrimitiveDataFloat(4, 1.f);
	}
	else
	{
		FactionMeshComp->SetCustomPrimitiveDataFloat(4, 0.f);
		ButtonMesh->SetCustomPrimitiveDataFloat(4, 0.f);
	}
}

bool AFactionPlatformButton::GetActive() const
{
	return bActive;
}

void AFactionPlatformButton::Trigger(bool bTriggered)
{
	if (!bActive)
	{
		return;
	}

	bButtonPressed = bTriggered;
	ButtonMesh->SetRelativeLocation(FVector(0.f, 0.f, bTriggered ? -PressedButtonOffset : 0.f));

	// CPD Index 4: Alpha
	FactionMeshComp->SetCustomPrimitiveDataFloat(4, bTriggered ? 1.0f : 0.0f);

	if (OnButtonTriggered.IsBound())
	{
		OnButtonTriggered.Broadcast(bTriggered);
	}
}

void AFactionPlatformButton::BeginPlay()
{
	Super::BeginPlay();

	SetActive(bStartActivated);

	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AFactionPlatformButton::OnOverlapping);
	TriggerComp->OnComponentEndOverlap.AddDynamic(this, &AFactionPlatformButton::OnOverlappingEnd);

	ButtonMesh->WeldTo(FactionMeshComp);
}

void AFactionPlatformButton::OnOverlapping(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& HitResult)
{
	if (bTriggerActive && Cast<ABasePawn>(OtherActor))
	{
		Trigger(true);
	}
}

void AFactionPlatformButton::OnOverlappingEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bTriggerActive && Cast<ABasePawn>(OtherActor))
	{
		Trigger(false);
	}
}
