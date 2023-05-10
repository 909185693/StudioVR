// Fill out your copyright notice in the Description page of Project Settings.


#include "GSXRInputFunctionLibrary.h"

bool UGSXRInputFunctionLibrary::GetControllerLinearAndAngularVelocity(EGSXR_ControllerType ControllerType, FVector &LinearVelocity, FVector &AngularVelocity, float WorldScale)
{
#if USE_GSXRAPI

	if (UGSXRInstance::GetInstance() != nullptr)
	{
		GSXR_DeviceType DeviceType;
		if (ControllerType == EGSXR_ControllerType::LeftHand) {
			DeviceType = GSXR_DeviceType::GSXR_DeviceType_Controller_Left;
		}
		else {
			DeviceType = GSXR_DeviceType::GSXR_DeviceType_Controller_Right;
		}
		UGSXRInstance::GetInstance()->GetControllerVelocityState(DeviceType, WorldScale, LinearVelocity, AngularVelocity);
		return true;
	}
#endif
	return false;
}
