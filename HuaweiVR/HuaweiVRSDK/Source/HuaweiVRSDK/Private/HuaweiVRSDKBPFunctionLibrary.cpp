// Fill out your copyright notice in the Description page of Project Settings.

#include "HuaweiVRSDKBPFunctionLibrary.h"
#include "HuaweiVRSDKAPI.h"
#include "HuaweiVRSDK.h"

#include "CoreMinimal.h"

bool UHuaweiVRSDKBPFunctionLibrary::GetHuaweiVRMessage(HuaweiVRMessageType& type, int& priority, int& boxType, TMap<FString, FString>& message) {
    int nativeMessageType = -1;

    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        return false;
    }

    if (huaweiVRSDK->GetHuaweiVRMessage(nativeMessageType, priority, boxType, message)) {
        if (NotifyEvent_NetWorkState == nativeMessageType || NOTIFICATION_NETWORKCHANGE_MESSAGE == nativeMessageType) {
            type = HuaweiVRMessageType::HuaweiVRMessage_NetWorkState;
        } else if (NotifyEvent_NewSMS == nativeMessageType || NOTIFICATION_SMS_MESSAGE == nativeMessageType) {
            type = HuaweiVRMessageType::HuaweiVRMessage_NewSMS;
        } else if (NotifyEvent_ThermalWarning == nativeMessageType) {
            type = HuaweiVRMessageType::HuaweiVRMessage_ThermalWarning;
        } else if (NotifyEvent_PhoneLowPower == nativeMessageType) {
            type = HuaweiVRMessageType::HuaweiVRMessage_PhoneLowPower;
        } else if (NotifyEvent_HelmetLowPower == nativeMessageType) {
            type = HuaweiVRMessageType::HuaweiVRMessage_HelmetLowPower;
        } else if (NOTIFICATION_GENERAL_MESSAGE == nativeMessageType) {
            type = HuaweiVRMessageType::HuaweiVRMessage_Common;
        }
        return true;
    }

    return false;
}

FString UHuaweiVRSDKBPFunctionLibrary::GetHuaweiVRSDKVersion() {
    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        return "";
    }

    return huaweiVRSDK->GetHuaweiVRSDKVersion();
}

int UHuaweiVRSDKBPFunctionLibrary::GetHMDInfo(HuaweiVRHelmetModel& helmetModel) {
    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        LOGI("UHuaweiVRSDKBPFunctionLibrary::GetHMDInfo GetHuaweiVRSDK is null");
        return -1;
    }
    HelmetModel iHelmetModel = HelmetUnknown;
    if (0 != huaweiVRSDK->GetHMDInfo(iHelmetModel)) {
        LOGI("UHuaweiVRSDKBPFunctionLibrary::GetHMDInfo GetHMDInfo error");
        return -1;
    }
    if (HelmetFirstGen == iHelmetModel) {
        helmetModel = HuaweiVRHelmetModel::HuaweiVRHelmet_FIRST_GEN;
    } else if (HelmetSecondGen == iHelmetModel) {
        helmetModel = HuaweiVRHelmetModel::HuaweiVRHelmet_SECOND_GEN;
    } else if (HelmetThirdGen == iHelmetModel) {
		helmetModel = HuaweiVRHelmetModel::HuaweiVRHelmet_Third_GEN;
	} else if (HelmetNotFound == iHelmetModel) {
        helmetModel = HuaweiVRHelmetModel::HuaweiVRHelmet_NOT_FOUND;
    } else {
        helmetModel = HuaweiVRHelmetModel::HuaweiVRHelmet_UNKNOWN;
    }
    return 0;
}

void UHuaweiVRSDKBPFunctionLibrary::GetHMDOrientationAndPosition(FQuat& Orientation, FVector& Position) {
    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        Orientation = FQuat(0.0f, 0.0f, 0.0f, 1.0f);
        Position = FVector(0.0f, 0.0f, 0.0f);
        return;
    }

    huaweiVRSDK->GetHMDOrientationAndPosition(Orientation, Position);
}

int UHuaweiVRSDKBPFunctionLibrary::ResetHMDYaw() {
    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        return -1;
    }

    return huaweiVRSDK->ResetYaw();
}

int UHuaweiVRSDKBPFunctionLibrary::SetHMDPoseLock(bool enable) {
    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        return -1;
    }

    return huaweiVRSDK->SetPoseLock(enable);
}

int UHuaweiVRSDKBPFunctionLibrary::SetChromaticAberration(bool enable) {
    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        return -1;
    }

    return huaweiVRSDK->SetChromaticAberration(enable);
}

int UHuaweiVRSDKBPFunctionLibrary::SetSvsEffect(bool enable) {
    FHuaweiVRSDK* huaweiVRSDK = FHuaweiVRSDK::GetHuaweiVRSDK();
    if (!huaweiVRSDK) {
        return -1;
    }

    return huaweiVRSDK->SetSvsEffect(enable);
}