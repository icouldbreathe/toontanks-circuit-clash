// GPL-2.0 license


#include "BaseFactionActor.h"

// Sets default values
ABaseFactionActor::ABaseFactionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FactionMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Faction Mesh"));
	FactionMeshComp->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ABaseFactionActor::BeginPlay()
{
	Super::BeginPlay();

	FactionData = LoadObject<UFactionConfigData>(nullptr, TEXT("/Game/Data/DA_FactionData.DA_FactionData"), nullptr,
		LOAD_None, nullptr);

	ChangeFactionColor_Implementation(OwningFaction);
}

// Called every frame
void ABaseFactionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseFactionActor::ChangeFactionColor_Implementation(EFaction Faction)
{
	OwningFaction = Faction;

	if (FactionData)
	{
		if (FLinearColor* ColorMapIterator = FactionData->FactionColorMap.Find(Faction))
		{
			TArray<UActorComponent*> ActorComponents;
			GetComponents(UStaticMeshComponent::StaticClass(), ActorComponents);

			for (UActorComponent* ActorComponent : ActorComponents)
			{
				UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(ActorComponent);

				// CPD Index 0,1,2,3: EmissiveColor(RGBA), 4: Alpha
				Mesh->SetCustomPrimitiveDataVector4(0, FVector4(
					ColorMapIterator->R,
					ColorMapIterator->G,
					ColorMapIterator->B,
					1.f
					));
				Mesh->SetCustomPrimitiveDataFloat(4, 1.f);
			}
		}
	}
}
