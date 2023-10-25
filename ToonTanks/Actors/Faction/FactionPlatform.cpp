// GPL-2.0 license


#include "FactionPlatform.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"

AFactionPlatform::AFactionPlatform()
{
	CornerMeshInstanced = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
		TEXT("Corner Mesh Instanced"));
	CornerMeshInstanced->SetupAttachment(SceneComponent);
}

void AFactionPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!CornerMesh)
	{
		return;
	}

	CornerMeshInstanced->ClearInstances();
	CornerMeshInstanced->SetStaticMesh(CornerMesh);

	for (int32 i = 0; i < 4; ++i)
	{
		if (BitmaskCornerFlags & (1 << i))
		{
			CornerMeshInstanced->AddInstance(FTransform(FRotator(0.f, 90.f * i, 0.0f)), false);
		}
		else
		{
			CornerMeshInstanced->RemoveInstance(i);
		}
	}
}
