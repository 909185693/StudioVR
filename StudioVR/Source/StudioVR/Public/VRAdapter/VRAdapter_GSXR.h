// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#if BUILD_VR_GSXR

#include "CoreMinimal.h"
#include "VRAdapter_Implementation.h"


class FVRAdapter_GSXR : public FVRAdapter_Implementation
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

	// 获取控制器类型
	virtual uint8 GetDeviceType() const;
};

#endif // BUILD_VR_GSXR

