// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "StudioVRAdapterInterface.h"
#include "StudioVRPrivate.h"

void IStudioVRAdapterInterface::RegisterAdapter(IStudioVRAdapterInterface* VRAdapter)
{
	check(!GVRAdapter);

	GVRAdapter = VRAdapter;
}