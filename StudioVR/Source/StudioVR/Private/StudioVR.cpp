// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVR.h"

#if BUILD_VR_HUAWEI
#include "VRAdapter/VRAdapter_Huawei.h"
#elif BUILD_VR_WAVE
#include "VRAdapter/VRAdapter_Wave.h"
#elif BUILD_VR_PICO
#include "VRAdapter/VRAdapter_Pico.h"
#elif BUILD_VR_OCULUS
#include "VRAdapter/VRAdapter_Oculus.h"
#elif BUILD_VR_STEAM
#include "VRAdapter/VRAdapter_Steam.h"
#endif


#define LOCTEXT_NAMESPACE "StudioVR"

IVRAdaptiveInterface* GVRAdapter = nullptr;

void FStudioVR::StartupModule()
{
#if BUILD_VR_HUAWEI
	GVRAdapter = new FVRAdapter_Huawei();
#elif BUILD_VR_WAVE
	GVRAdapter = new FVRAdapter_Wave();
#elif BUILD_VR_PICO
	GVRAdapter = new FVRAdapter_Pico();
#elif BUILD_VR_OCULUS
	GVRAdapter = new FVRAdapter_Oculus();
#elif BUILD_VR_STEAM
	GVRAdapter = new FVRAdapter_Steam();
#endif
}

void FStudioVR::ShutdownModule()
{
	if (GVRAdapter != nullptr)
	{
		delete GVRAdapter;
		GVRAdapter = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStudioVR, StudioVR)


DEFINE_LOG_CATEGORY(LogStudioVR)