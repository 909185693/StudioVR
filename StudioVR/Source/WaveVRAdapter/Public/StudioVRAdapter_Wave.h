// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#define WAVEVR_WINDOWS_TEST 1

#include "CoreMinimal.h"
#include "StudioVRAdapter_Implementation.h"


class FStudioVRAdapter_Wave : public FStudioVRAdapter_Implementation
{
public:
	// ����
	virtual void Construction(UObject* Outer, const class FObjectInitializer& ObjectInitializer) override;
	
	// ע��
	virtual void Register(AStudioVRPawn* NewPawn) override;
	virtual void Unregister() override;

	// Tick Function
	virtual void Tick(float DeltaTime) override;
	virtual bool PollControllerState(EControllerHand TrackingSource, FVector& Position, FRotator& Orientation) override;

	// ������ʾģ��
	virtual void AdaptationDisplayComponent(class UAdaptiveControllerComponent* AdaptiveControllerComponent) override;

	// ��ȡ���������ƫ��
	virtual FVector GetCameraOffset() const;
private:
#if (PLATFORM_ANDROID || WAVEVR_WINDOWS_TEST)
	class UWaveVREventCommon* WaveVREventCommon;

	void ApplicationWillEnterBackground();
#endif

private:
	bool bWaveVRSetupDone;
};

