// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLoading.h"
#include "Components/Widget.h"
#include "Loading.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ULoading : public UWidget
{
	GENERATED_BODY()

public:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	FSlateBrush RotatingBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	FSlateBrush BackgroundBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation")
	float RotationSpeed = 0.2f;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SLoading> LoadingSlate;
};
