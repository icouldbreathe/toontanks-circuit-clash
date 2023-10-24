// Fill out your copyright notice in the Description page of Project Settings.


#include "Loading.h"

#define LOCTEXT_NAMESPACE "UMG"

void ULoading::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	LoadingSlate->SetRotatingBrush(&RotatingBrush);
	LoadingSlate->SetBackgroundBrush(&BackgroundBrush);
	LoadingSlate->SetRotationSpeed(RotationSpeed);
}

void ULoading::ReleaseSlateResources(bool bReleaseChildren)
{
	LoadingSlate.Reset();
}

TSharedRef<SWidget> ULoading::RebuildWidget()
{
	LoadingSlate = SNew(SLoading)
	.RotatingBrush(&RotatingBrush)
	.BackgroundBrush(&BackgroundBrush)
	.RotationSpeed(RotationSpeed);
	return LoadingSlate.ToSharedRef();
}

#if WITH_EDITOR
const FText ULoading::GetPaletteCategory()
{
	return LOCTEXT("CustomPaletteCategory", "ToonTanks Slate");
}
#endif
