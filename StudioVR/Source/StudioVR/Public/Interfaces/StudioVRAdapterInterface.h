// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "StudioVRPawn.h"


namespace EDeviceType
{
	static const uint8 Defualt = 0;

	static const uint8 Oculus_Quest = 1;
	static const uint8 Oculus_Quest2 = 2;

	static const uint8 HTC_Vive = 3;
	static const uint8 HTC_Vive_Cosmos = 4;
	static const uint8 HTC_Vive_Focus = 5;
	static const uint8 HTC_Vive_Focus_Plus = 6;

	static const uint8 Pico_G2 = 7;
	static const uint8 Pico_G2_4K = 8;
	static const uint8 Pico_Noe = 9;
	static const uint8 Pico_Noe2 = 10;
	static const uint8 Pico_Noe3 = 11;

	static const uint8 Vive_Gun = 12;
	static const uint8 PP_Gun = 13;

	static const uint8 Nolo = 14;

	static const uint8 GSXR = 15;
};

class STUDIOVR_API IStudioVRAdapterInterface
{
public:
	virtual ~IStudioVRAdapterInterface() {}

public:
	// 构造
	virtual void Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer) = 0;

	// 注册
	virtual void Register(AStudioVRPawn* NewPawn) = 0;
	virtual void Unregister() = 0;

	// Tick Function
	virtual void Tick(float DeltaTime) = 0;
	virtual bool PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation) = 0;

	// 适配显示模型
	virtual void AdaptationDisplayComponent(class UAdaptiveControllerComponent* AdaptiveControllerComponent) = 0;

	// 获取设配摄像机偏移
	virtual FVector GetCameraOffset() const = 0;

	// 获取控制器类型
	virtual uint8 GetDeviceType() const = 0;

	// 注册设配器
	static void RegisterAdapter(IStudioVRAdapterInterface* VRAdapter);
};
