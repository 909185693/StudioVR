// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#if BUILD_VR_GSXR

#include "CoreMinimal.h"
#include "VRAdapter_Implementation.h"


class FVRAdapter_GSXR : public FVRAdapter_Implementation
{
public:
	// ����/����
	virtual void Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer) override;
	
	// ע��
	virtual void Register(AStudioVRPawn* NewPawn) override;
	virtual void Unregister() override;

	// Tick Function
	virtual void Tick(float DeltaTime) override;
	virtual bool PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation) override;

	// ������ʾģ��
	virtual void AdaptationDisplayComponent(class UAdaptiveControllerComponent* AdaptiveControllerComponent) override;

	// ��ȡ����������
	virtual uint8 GetDeviceType() const;
};

#endif // BUILD_VR_GSXR

