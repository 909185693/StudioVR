// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AdaptiveControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "StudioVR.h"


void UAdaptiveControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GVRAdapter != nullptr)
	{
		GVRAdapter->AdaptationDisplayComponent(this);
	}
}

void UAdaptiveControllerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GVRAdapter && IsActive())
	{
		FVector OutPosition;
		FRotator OutOrientation;

		const EControllerHand ControllerHand = GetTrackingSource();

		const bool bNewTrackedState = GVRAdapter->PollControllerState(ControllerHand, OutPosition, OutOrientation);
		if (bNewTrackedState)
		{
			SetRelativeLocationAndRotation(OutPosition, OutOrientation);

			//UE_LOG(LogStudioVR, Log, TEXT("UAdaptiveControllerComponent::TickComponent OutPosition[%s] OutOrientation[%s]"), *OutPosition.ToString(), *OutOrientation.ToString());
		}
	}

	bool bSouldControllerModel = bDisplayDeviceModel && IsActive();
	if (bDisplayCustomDeviceModel != bSouldControllerModel)
	{
		bDisplayCustomDeviceModel = bSouldControllerModel;

		TArray<USceneComponent*> Children;

		GetChildrenComponents(true, Children);
		for (USceneComponent* ChildComponent : Children)
		{
			if (ChildComponent && ChildComponent->IsA(UStaticMeshComponent::StaticClass()) && ChildComponent->GetName().Contains(TEXT("_RENDER")))
			{
				ChildComponent->SetHiddenInGame(!bDisplayCustomDeviceModel);
			}
		}
	}
}