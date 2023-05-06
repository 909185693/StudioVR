//
// Created by xiaoyang on 2021/8/3.
//

#ifndef GSXR_APP_V1_GSXR_INPUT_TYPES_H
#define GSXR_APP_V1_GSXR_INPUT_TYPES_H

#include "gsxr_types.h"
#include "gsxr_stdinc.h"
#include <stdint.h>
#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

#if !defined(GSXR_DEFINE_INPUT_HANDLE)
#define GSXR_DEFINE_INPUT_HANDLE(object) typedef uint64_t object;
#endif

GSXR_DEFINE_INPUT_HANDLE(GSXR_XrActionSetHandle_t)
GSXR_DEFINE_INPUT_HANDLE(GSXR_XrActionHandle_t)
GSXR_DEFINE_INPUT_HANDLE(GSXR_XrInputValueHandle_t)

GSXR_DEFINE_ATOM(GSXR_TrackedDeviceId)    // GSXR 显示标识号
typedef struct GSXR_XrActiveActionSet{
    GSXR_XrActionSetHandle_t          ulActionSet;
    GSXR_XrInputValueHandle_t         ulRestrictedToDevice;
    int32_t                           nPriorty;
}GSXR_XrActiveActionSet;

typedef struct GSXR_XrInputDigitalActionData{
    bool                              bActive;
    GSXR_XrInputValueHandle_t         activeOrigin;
    bool                              bState;
    bool                              bChanged;
    GSXR_Time                         updateTime;
}GSXR_XrInputDigitalActionData;

typedef struct GSXR_XrInputAnalogActionData{
    bool                              bActive;
    GSXR_XrInputValueHandle_t         activeOrigin;
    float                             x,y,z;
    float                             deltaX,deltaY,deltaZ;
    GSXR_Time                         updateTime;
}GSXR_XrInputAnalogActionData;

extern GSXR_EXPORT GSXR_Result  GSXR_XrActionManifestPath(GSXR_Runtime runtime,const char* pchActionManifestPath);

extern GSXR_EXPORT GSXR_Result  GSXR_GetXrActionSetHandle(GSXR_Runtime runtime,const char* pchActionSetName,GSXR_XrActionSetHandle_t *pHandle);

extern GSXR_EXPORT GSXR_Result  GSXR_GetXrActionHandle(GSXR_Runtime runtime,const char* pchActionName,GSXR_XrActionHandle_t* pHandle);

extern GSXR_EXPORT GSXR_Result  GSXR_GetXrInputSourceHandle(GSXR_Runtime runtime,const char* pchInputSourcePath,GSXR_XrInputValueHandle_t* pHandle);

extern GSXR_EXPORT GSXR_Result  GSXR_UpdateXrActionState(GSXR_Runtime runtime,GSXR_XrActiveActionSet *pSets,uint32_t unSizeOfActionSet_t,uint32_t unSetCount);

extern GSXR_EXPORT GSXR_Result  GSXR_GetXrActionDigitalData(GSXR_Runtime runtime,GSXR_XrActionHandle_t action,GSXR_XrInputDigitalActionData *pActionData,uint32_t unActionDataSize,GSXR_XrInputValueHandle_t ulRestrictToDevice);

extern GSXR_EXPORT GSXR_Result  GSXR_GetXrActionAnalogData(GSXR_Runtime runtime,GSXR_XrActionHandle_t action,GSXR_XrInputAnalogActionData *pActionData,uint32_t unActionDataSize,GSXR_XrInputValueHandle_t ulRestrictToDevice);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif //GSXR_APP_V1_GSXR_INPUT_TYPES_H
