// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class HuaweiVRSDKVolume
{
public:
	HuaweiVRSDKVolume();
	~HuaweiVRSDKVolume();

    void SubmitVolumeTextures();
private:
#if PLATFORM_ANDROID
    uint mLeftVolumeTexId;
    uint mRightVolumeTexId;

    bool mNeedToDraw;
    int mLastVolume;
    double mVolumeChangeTime;
    bool mShowVolume;
#endif
};
