// Fill out your copyright notice in the Description page of Project Settings.


#include "GSXRBlueprintFunctionLibrary.h"
#include "GSXRHMD.h"
#include "Engine.h"
#include "GSXRSetting.h"
FGSXRDeviceStateChangeDelegate UGSXRBlueprintFunctionLibrary::DeviceStateChangedDelegate;

static FGSXRHMD* GetGsxrHMD()
{
	if (GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetSystemName() == FName("GSXRHMD"))
	{

		return static_cast<FGSXRHMD*>(GEngine->XRSystem.Get());
	}
	UE_LOG(LogTemp, Error, TEXT("UGSXRBlueprintFunctionLibrary GetGsxrHMD null"));
	return nullptr;
}

UGSXRBlueprintFunctionLibrary::UGSXRBlueprintFunctionLibrary()
{
	UGSXRInstance::GetInstance()->DeviceStateChangedDelegate.AddDynamic(this,&UGSXRBlueprintFunctionLibrary::DeviceConnectStateChange);
}

void UGSXRBlueprintFunctionLibrary::OnDeviceConnectStateChange(	FGSXRDeviceStateChangeDelegate OnDeviceStateChangedDelegate)
{
	DeviceStateChangedDelegate = OnDeviceStateChangedDelegate;
}

void UGSXRBlueprintFunctionLibrary::DeviceConnectStateChange(int32 DeviceType)
{
	EGSXR_DeviceType ChangeDevice = EGSXR_DeviceType::GSXR_HMD;
	switch (DeviceType)
	{	
	case 1:
		case 2:
			ChangeDevice = EGSXR_DeviceType::GSXR_HMD;
			break;
		case 3:
			ChangeDevice = EGSXR_DeviceType::GSXR_Right_Controller;
			break;
		case 4:
			ChangeDevice = EGSXR_DeviceType::GSXR_Left_Controller;
			break;
		default:
			break;
	}
	DeviceStateChangedDelegate.ExecuteIfBound(ChangeDevice);
	
}

bool UGSXRBlueprintFunctionLibrary::GetDeviceBatteryState(EGSXR_DeviceType Device, bool& bCharging, float& Battery)
{
#if USE_GSXRAPI
	if ( UGSXRInstance::GetInstance() != nullptr)
	{
		GSXR_DeviceType DeviceType;
		switch (Device)
		{
			case EGSXR_DeviceType::GSXR_HMD:
				DeviceType = GSXR_DeviceType_HMD_VR;
				break;
			case EGSXR_DeviceType::GSXR_Left_Controller:
				DeviceType = GSXR_DeviceType_Controller_Left;
				break;
			default:
				DeviceType = GSXR_DeviceType_Controller_Right;
				break;
		}
		 UGSXRInstance::GetInstance()->GetBatteryStatus(DeviceType,Battery,bCharging);
		return true;
	}
#endif
	return false;
}

bool UGSXRBlueprintFunctionLibrary::GetDeviceConnectState(EGSXR_DeviceType Device, bool& bConnected)
{
	
#if USE_GSXRAPI
	if ( UGSXRInstance::GetInstance() != nullptr)
	{
		GSXR_DeviceType DeviceType;
		switch (Device)
		{
		case EGSXR_DeviceType::GSXR_HMD:
			DeviceType = GSXR_DeviceType_HMD_VR;
			break;
		case EGSXR_DeviceType::GSXR_Left_Controller:
			DeviceType = GSXR_DeviceType_Controller_Left;
			break;
		default:
			DeviceType = GSXR_DeviceType_Controller_Right;
			break;
		}
		UGSXRInstance::GetInstance()->GetDeviceConnectState(DeviceType,bConnected);
		return true;
	}
#endif
	return false;
}

bool UGSXRBlueprintFunctionLibrary::EnableFoveatedRender(bool bEnable)
{
	static UGSXRSettings* Settings = GetMutableDefault<UGSXRSettings>();
	if (Settings)
	{
		Settings->bEnableFoveated = bEnable;
		return true;
	}
	return false;
}

bool UGSXRBlueprintFunctionLibrary::SetFoveatedRenderParameters(EGSXR_FoveatedLevel FoveatedLevel,
	float FovUpPercentage, float FovDownPercentage, float FovLeftPercentage, float FovRightPercentage)
{
	static UGSXRSettings* Settings = GetMutableDefault<UGSXRSettings>();
	if (Settings)
	{
		Settings->FoveationLevel = static_cast<EFoveationLevel::Type> (static_cast<int32>(FoveatedLevel));
		Settings->Fov_Percentage_Up = FovUpPercentage;
		Settings->Fov_Percentage_Down = FovDownPercentage;
		Settings->Fov_Percentage_Left = FovLeftPercentage;
		Settings->Fov_Percentage_Right = FovRightPercentage;
		return true;
	}
	return false;
}

bool UGSXRBlueprintFunctionLibrary::GetFoveatedRenderState(bool& bEnable)
{
	static UGSXRSettings* Settings = GetMutableDefault<UGSXRSettings>();
	if (Settings)
	{
		bEnable = Settings->bEnableFoveated;
		return true;
	}
	return false;
}

bool UGSXRBlueprintFunctionLibrary::GetFoveatedRenderParameters(EGSXR_FoveatedLevel& FoveatedLevel,
	float& FovUpPercentage, float& FovDownPercentage, float& FovLeftPercentage, float& FovRightPercentage)
{
	static UGSXRSettings* Settings = GetMutableDefault<UGSXRSettings>();
	if (Settings)
	{
		FoveatedLevel = static_cast<EGSXR_FoveatedLevel>(static_cast<int32>(Settings->FoveationLevel));
		FovUpPercentage= Settings->Fov_Percentage_Up;
		FovDownPercentage = Settings->Fov_Percentage_Down;
		FovLeftPercentage = Settings->Fov_Percentage_Left;
		FovRightPercentage = Settings->Fov_Percentage_Right;
		return true;
	}
	return false;
}

bool UGSXRBlueprintFunctionLibrary::SetPrimaryDeviceType(EGSXR_DeviceType Device)
{
#if USE_GSXRAPI
	if ( UGSXRInstance::GetInstance() != nullptr)
	{
		GSXR_DeviceType DeviceType;
		switch (Device)
		{
		case EGSXR_DeviceType::GSXR_HMD:
			return false;
		case EGSXR_DeviceType::GSXR_Left_Controller:
			DeviceType = GSXR_DeviceType_Controller_Left;
			break;
		default:
			DeviceType = GSXR_DeviceType_Controller_Right;
			break;
		}
		UGSXRInstance::GetInstance()->SetPrimaryDeviceType(DeviceType);
		return true;
	}
#endif
	return false;
}

bool UGSXRBlueprintFunctionLibrary::GetPrimaryDeviceType(EGSXR_DeviceType& Device)
{
#if USE_GSXRAPI
	if ( UGSXRInstance::GetInstance() != nullptr)
	{
		GSXR_Flags64 DeviceType;
		UGSXRInstance::GetInstance()->GetPrimaryDeviceType(DeviceType);
		switch (DeviceType)
		{
		case GSXR_DeviceType_HMD_VR:
			Device = EGSXR_DeviceType::GSXR_HMD;
			break;
		case GSXR_DeviceType_Controller_Left:
			Device = EGSXR_DeviceType::GSXR_Left_Controller;
			break;
		case GSXR_DeviceType_Controller_Right:
			Device = EGSXR_DeviceType::GSXR_Right_Controller;
			break;
		default:
			return false;
		}	
		return true;
	}
#endif
	return false;
}

FQuat UGSXRBlueprintFunctionLibrary::GsxrGetCurrentOrientation()
{
#if USE_GSXRAPI
	
	return  GetGsxrHMD()->CurrentFrame_RenderThread.Orientation;
#else
	return FQuat(0, 0, 0, 0);
#endif
}

FVector UGSXRBlueprintFunctionLibrary::GsxrGetCurrentPosition()
{
#if USE_GSXRAPI
	return  GetGsxrHMD()->CurrentFrame_RenderThread.Position;
#else
	return FVector(0, 0, 0);
#endif
}

bool UGSXRBlueprintFunctionLibrary::GSXRSetSystemSetting(EGSXR_XrSystemSettingType setting)
{	
#if USE_GSXRAPI
	if (UGSXRInstance::GetInstance() != nullptr)
	{
		GSXR_XrSystemSettingType settingType;
		switch (setting)
		{
		case EGSXR_XrSystemSettingType::EXrSystemSettingType_Wifi:
			settingType = GSXR_XrSystemSettingType_Wifi;
			break;
		case EGSXR_XrSystemSettingType::EXrSystemSettingType_Bluetooth:
			settingType = GSXR_XrSystemSettingType_Bluetooth;
			break;
		default:			
			break;
		}		
		UGSXRInstance::GetInstance()->SetSystemSetting(settingType);
		return true;
	}
#endif
	return false;
}

