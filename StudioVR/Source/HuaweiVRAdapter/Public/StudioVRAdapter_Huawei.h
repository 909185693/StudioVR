// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StudioVRAdapter_Implementation.h"
#include "HVRControllerFunctionLibrary.h"


class FStudioVRAdapter_Huawei : public FStudioVRAdapter_Implementation
{
public:
	// 构造/析构
	virtual void Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer) override;
	
	// 注册
	virtual void Register(AStudioVRPawn* NewPawn) override;
	virtual void Unregister() override;

	// Tick Function
	virtual void Tick(float DeltaTime) override;
	virtual bool PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation) override;

	// 适配显示模型
	virtual void AdaptationDisplayComponent(class UAdaptiveControllerComponent* AdaptiveControllerComponent) override;

private:
	float UE_HANDOFFSET_ANGLE = 25.f;
	FVector UE_HEADTOELBOW_OFFSET = FVector(0, 0.f, -70.f);
	FVector UE_ELBOWTOWRIST_OFFSET = FVector(25.f, 0.0f, 0.0f);
	FVector UE_ELBOW_PITCH_OFFSET = FVector(0.f, 0.f, 80.f);

	float UE_ELBOW_PITCH_ANGLE_MIN = -90.f;
	float UE_ELBOW_PITCH_ANGLE_MAX = 90.f;
	const float ELBOW_TO_XYPLANE_LERP_MIN = 0.45f;
	const float ELBOW_TO_XYPLANE_LERP_MAX = 0.65f;

	const FVector UE_FORWARD = FVector(1, 0, 0);
	const FVector UE_RIGHT = FVector(0, 1, 0);
	const FVector UE_UP = FVector(0, 0, 1);

	HVRControllerType GetHVRControllerType(EControllerHand TrackingSource) const;

	void GetControllerSimulationRotationAndLocation(const FVector& HMDLocation, const FQuat& HMDRotation, const FQuat& PoseRotation, bool bLeftOrRight, FRotator& OutRotation, FVector& OutLocation) const;
};

