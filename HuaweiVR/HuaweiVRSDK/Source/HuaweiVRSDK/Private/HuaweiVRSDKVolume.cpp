// Fill out your copyright notice in the Description page of Project Settings.

#include "HuaweiVRSDKVolume.h"
#include "HuaweiVRSDKAPI.h"

#if PLATFORM_ANDROID
#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"
#include "HuaweiVRSDKJNI.h"
#endif

double GetCurrentTime() {
#if PLATFORM_ANDROID
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / (double)1000000;
#else
    return 0;
#endif
}

HuaweiVRSDKVolume::HuaweiVRSDKVolume()
#if PLATFORM_ANDROID
    : mLeftVolumeTexId(0), mRightVolumeTexId(0), mLastVolume(-1), mVolumeChangeTime(0), mShowVolume(false)
#endif
{
#if PLATFORM_ANDROID
    if (0 == mLeftVolumeTexId) {
        glGenTextures(1, &mLeftVolumeTexId);
        glBindTexture(GL_TEXTURE_2D, mLeftVolumeTexId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE wrong changed GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE wrong changed

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (0 == mRightVolumeTexId) {
        glGenTextures(1, &mRightVolumeTexId);
        glBindTexture(GL_TEXTURE_2D, mRightVolumeTexId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE wrong changed GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE wrong changed

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
#endif
}

HuaweiVRSDKVolume::~HuaweiVRSDKVolume()
{
#if PLATFORM_ANDROID
    HVR_SendMessage("isShowVolume", "False");
    LOGI("HuaweiVRSDKVolume::~HuaweiVRSDKVolume HVR_SendMessage False");
    if (0 != mLeftVolumeTexId) {
        glDeleteTextures(1, &mLeftVolumeTexId);
        mLeftVolumeTexId = 0;
    }

    if (0 != mRightVolumeTexId) {
        glDeleteTextures(1, &mRightVolumeTexId);
        mRightVolumeTexId = 0;
    }
#endif
}

void HuaweiVRSDKVolume::SubmitVolumeTextures() {
#if PLATFORM_ANDROID
    bool needToSubmit = true;
    int volume = HVR_GetVolume();
    //LOGI("HuaweiVRSDKVolume::SubmitVolumeTextures volume %d", volume);
    if (volume >= 0) {
        if (volume != mLastVolume) {
            mNeedToDraw = true;
            if (!mShowVolume) {
                mShowVolume = true;
                HVR_SendMessage("isShowVolume", "True");
                LOGI("HuaweiVRSDKVolume::SubmitVolumeTextures HVR_SendMessage True");
            }
            if (mLastVolume >= 0) {
                mVolumeChangeTime = GetCurrentTime();
            }
            mLastVolume = volume;

            LOGI("HuaweiVRSDKVolume::SubmitVolumeTextures volume change time %f", mVolumeChangeTime);
        }
    }

    if (GetCurrentTime() - mVolumeChangeTime > 3.0) {
        needToSubmit = false;
        if (mShowVolume) {
            mShowVolume = false;
            HVR_SendMessage("isShowVolume", "False");
            LOGI("HuaweiVRSDKVolume::SubmitVolumeTextures HVR_SendMessage False");
        }
        //LOGI("HuaweiVRSDKVolume::SubmitVolumeTextures DeltaTime > 3");
    }

    if (needToSubmit) {
        if (mNeedToDraw) {
            mNeedToDraw = false;
            DrawVolumeTexture(mLeftVolumeTexId, mRightVolumeTexId, volume);
        }

        LOGI("DrawVolumeTexture texid %d %d", mLeftVolumeTexId, mRightVolumeTexId);

        HVR_CameraEndFrame(0, mLeftVolumeTexId, 1);
        HVR_CameraEndFrame(1, mRightVolumeTexId, 1);
    }
#endif
}
