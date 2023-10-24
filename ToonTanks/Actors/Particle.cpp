// Fill out your copyright notice in the Description page of Project Settings.


#include "Particle.h"

#include "NiagaraComponent.h"

// Sets default values
AParticle::AParticle()
{
	PrimaryActorTick.bCanEverTick = false;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	RootComponent = NiagaraComp;
}

UNiagaraComponent* AParticle::GetNiagaraComponent() const
{
	return NiagaraComp;
}

void AParticle::Attach(USceneComponent* SceneComponent, FName SocketName)
{
	AttachToComponent(SceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);

	if (Owner && GetOwner()->OnDestroyed.IsAlreadyBound(this, &AParticle::OnParticleOwnerDestroyed))
	{
		GetOwner()->OnDestroyed.RemoveDynamic(this, &AParticle::OnParticleOwnerDestroyed);
	}

	SetOwner(SceneComponent->GetOwner());
	GetOwner()->OnDestroyed.AddDynamic(this, &AParticle::OnParticleOwnerDestroyed);
}

void AParticle::Detach()
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	GetOwner()->OnDestroyed.RemoveDynamic(this, &AParticle::OnParticleOwnerDestroyed);
	SetOwner(nullptr);
}

void AParticle::Kill() const
{
	NiagaraComp->DeactivateImmediate();
}

void AParticle::Restart()
{
	NiagaraComp->ResetSystem();
}

void AParticle::Complete()
{
	NiagaraComp->Deactivate();
}

void AParticle::DetachAndComplete()
{
	Detach();
	Complete();
}

void AParticle::SetNiagaraSystem(UNiagaraSystem* NiagaraSystem)
{
	if (NiagaraSystem)
	{
		NiagaraComp->SetAsset(NiagaraSystem);
		NiagaraComp->Activate();
	}
}

void AParticle::SetLifetimeRule(EParticleLifetimeRule InRule)
{
	LifetimeRule = InRule;
}

// Called when the game starts or when spawned
void AParticle::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComp->OnSystemFinished.AddDynamic(this, &AParticle::ParticleSystemFinished);
}

// Called every frame
void AParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParticle::ParticleSystemFinished(UNiagaraComponent* NiagaraComponent)
{
	Destroy();
}

void AParticle::OnParticleOwnerDestroyed(AActor* ActorDestroyed)
{
	switch (LifetimeRule)
	{
		case EParticleLifetimeRule::DiesOnOwnerDestroyed:
			Kill();
			break;
		case EParticleLifetimeRule::DetachesAndContinuesOnOwnerDestroyed:
			Detach();
			break;
		case EParticleLifetimeRule::DetachesAndDeactivatesOnOwnerDestroyed:
			DetachAndComplete();
			break;
	}
}
