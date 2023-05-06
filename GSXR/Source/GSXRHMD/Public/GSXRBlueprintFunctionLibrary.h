// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSXRInstance.h"
#include "GSXRSetting.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GSXRBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EGSXR_DeviceType:uint8
{
	GSXR_HMD,
	GSXR_Left_Controller,
	GSXR_Right_Controller,
};

UENUM(BlueprintType)
enum class EGSXR_FoveatedLevel:uint8
{
	EFoveationLevel_System		UMETA(DisplayName = "System"),
	EFoveationLevel_High		UMETA(DisplayName = "High"),
	EFoveationLevel_Medium		UMETA(DisplayName = "Medium"),
	EFoveationLevel_Low			UMETA(DisplayName = "Low"),
};

UENUM(BlueprintType)
enum class EGSXR_XrSystemSettingType :uint8
{
	EXrSystemSettingType_Wifi			UMETA(DisplayName = "Wifi"),
	EXrSystemSettingType_Bluetooth		UMETA(DisplayName = "Bluetooth"),
};

UENUM()
enum class EGSXR_ApplicationType:uint8
{
	ApplicationType_VR		UMETA(DisplayName = "VR"),
	ApplicationType_AR		UMETA(DisplayName = "AR"),
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FGSXRDeviceStateChangeDelegate, const EGSXR_DeviceType&, Device);
/**
 * 
 */
UCLASS()
class GSXRHMD_API UGSXRBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UGSXRBlueprintFunctionLibrary();

	static FGSXRDeviceStateChangeDelegate DeviceStateChangedDelegate;
	
	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
		static void OnDeviceConnectStateChange(FGSXRDeviceStateChangeDelegate OnDeviceStateChangedDelegate);
	
	UFUNCTION()
	void DeviceConnectStateChange(int32 DeviceType);
	/**
	 *Get the battery status of the device
	 *
	 *@param Device				(in) Specify the device type, for example, left-controller or right-controller
	 *@param bCharging			(out)Whether the device is charging
	 *@param Battery			(out)battery of the device
	 */
	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool GetDeviceBatteryState(EGSXR_DeviceType Device,bool &bCharging,float& Battery);

	/**
	*Get the connect state of the device
	*
	*@param Device				(in) Specify the device type, for example, left-controller or right-controller
	*@param bConnected			(out)Whether the device is connected
	*/
	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool GetDeviceConnectState(EGSXR_DeviceType Device,bool& bConnected);

	/**
	*Enables/disables foveated rendering.
	*/
	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool EnableFoveatedRender(bool bEnable);

	/**
	*Set the parameters for foveated rendering.
	*
	*@param FoveatedLevel		(in) the level of foveated rendering
	*@param FovUpPercentage		(in) the percentage of up fov
	*@param FovDownPercentage	(in) the percentage of down fov
	*@param FovLeftPercentage	(in) the percentage of left fov
	*@param FovRightPercentage	(in) the percentage of right fov
	*/
	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool SetFoveatedRenderParameters(EGSXR_FoveatedLevel FoveatedLevel,float FovUpPercentage, float FovDownPercentage,float FovLeftPercentage,float FovRightPercentage);

	/**
	*Get the foveated rendering state.
	*/
	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool GetFoveatedRenderState(bool &bEnable);

	/**
	*Get the parameters for foveated rendering.
	*
	*@param FoveatedLevel		(out) the level of foveated rendering
	*@param FovUpPercentage		(out) the percentage of up fov
	*@param FovDownPercentage	(out) the percentage of down fov
	*@param FovLeftPercentage	(out) the percentage of left fov
	*@param FovRightPercentage	(out) the percentage of right fov
	*/
	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool GetFoveatedRenderParameters(EGSXR_FoveatedLevel &FoveatedLevel,float &FovUpPercentage, float &FovDownPercentage,float &FovLeftPercentage,float &FovRightPercentage);

	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool SetPrimaryDeviceType(EGSXR_DeviceType Device);

	UFUNCTION(BlueprintCallable,Category="GSXR Interface")
	static bool GetPrimaryDeviceType(EGSXR_DeviceType &Device);


	UFUNCTION(BlueprintCallable, Category = "GSXR Interface")
		static FQuat GsxrGetCurrentOrientation();

	UFUNCTION(BlueprintCallable, Category = "GSXR Interface")
		static FVector GsxrGetCurrentPosition();

	UFUNCTION(BlueprintCallable, Category = "GSXR Interface")
		static bool GSXRSetSystemSetting(EGSXR_XrSystemSettingType setting);
};

