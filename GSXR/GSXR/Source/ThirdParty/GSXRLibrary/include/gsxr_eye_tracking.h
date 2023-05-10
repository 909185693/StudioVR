#ifndef GSXR_EYE_TRACKING_H_
#define GSXR_EYE_TRACKING_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举眼球追踪特性之功能选项
typedef enum GSXR_EyeTrackingOptions {
    GSXR_EyeTrackingOptions_Eye_Left            = 0x00000001,   // 左眼追踪
    GSXR_EyeTrackingOptions_Eye_Right           = 0x00000002,   // 右眼追踪

    GSXR_EyeTrackingOptions_Eye_Motion          = 0x00010000,   // 实体眼球追踪
    GSXR_EyeTrackingOptions_Gaze_Interaction    = 0x00020000,   // 注视点追踪
} GSXR_EyeTrackingOptions;

// ------------------------------------
// GSXR_API_眼球追踪类
// ------------------------------------

// 枚举眼球数据类型
typedef enum GSXR_EyeFactor {
    GSXR_EyeFactor_Pose       = 0x00000001,   // 眼球位姿
    GSXR_EyeFactor_Pupil      = 0x00000002,   // 瞳孔数据
    GSXR_EyeFactor_Focal      = 0x00000004,   // 眼球焦点位置
    GSXR_EyeFactor_Openness   = 0x00000008,   // 眼睛开合度
} GSXR_EyeFactor;

// 眼球位姿结构体
typedef struct GSXR_EyePose {
    void*           next;       // 预留指针
    GSXR_Bool32     isValid;    // GSXR_TRUE 表示位姿有效, GSXR_FALSE 表示位姿无效
    GSXR_Posef      pose;       // 眼球位姿 (本地坐标系, 以两眼间之中心点面向正前方为原点)
} GSXR_EyePose;

// 瞳孔数据结构体
typedef struct GSXR_EyePupil {
    void*           next;       // 预留指针
    GSXR_Bool32     isValid;    // GSXR_TRUE 表示直径有效, GSXR_FALSE 表示直径无效
    float           diameter;   // 瞳孔直径, 单位为毫米 mm
} GSXR_EyePupil;

// 眼球焦点位置结构体 (归一化数据)
typedef struct GSXR_EyeFocal {
    void*           next;       // 预留指针
    GSXR_Bool32     isValid;    // GSXR_TRUE 表示位置有效, GSXR_FALSE 表示位置无效
    GSXR_Vector2f   point;      // 眼球焦点位置坐标, 数据范围为 -1 <= x <= 1, -1 <= y <= 1
} GSXR_EyeFocal;

// 眼睛开合度结构体 (归一化数据)
typedef struct GSXE_EyeOpenness {
    void*           next;       // 预留指针
    GSXR_Bool32     isValid;    // GSXR_TRUE 表示开合度有效, GSXR_FALSE 表示开合度无效
    float           openness;   // 眼睛开合度, 数据范围为 0 - 1, 0 表示闭眼
} GSXE_EyeOpenness;

// 眼球数据结构体
typedef struct GSXR_EyeEntityData {
    void*               next;       // 预留指针
    GSXR_Bool32         onTracking; // GSXR_TRUE 表示眼球追踪中, GSXR_FALSE 表示未起始眼球追踪
    GSXR_EyePose        pose;       // 眼球位姿
    GSXR_EyePupil       pupil;      // 瞳孔数据
    GSXR_EyeFocal       focal;      // 眼球焦点位置
    GSXE_EyeOpenness    openness;   // 眼睛开合度
} GSXR_EyeEntityData;

// 注视位姿结构体
typedef struct GSXR_EyeGaze {
    void*           next;       // 预留指针
    GSXR_Bool32     isValid;    // GSXR_TRUE 表示位姿有效, GSXR_FALSE 表示位姿无效
    GSXR_Posef      pose;       // 位姿 (世界坐标系)
} GSXR_EyeGaze;

// 注视数据结构体
typedef struct GSXR_EyeGazeData {
    void*           next;           // 预留指针
    GSXR_Bool32     onTracking;     // GSXR_TRUE 表示注视点追踪中, GSXR_FALSE 表示未起始注视点追踪
    GSXR_EyeGaze    gazePose;       // 注视位姿
} GSXR_EyeGazeData;

// 眼球追踪数据结构体
typedef struct GSXR_EyeTrackingData {
    void*               next;       // 预留指针
    GSXR_EyeEntityData  left;       // 左眼球数据
    GSXR_EyeEntityData  right;      // 右眼球数据
    GSXR_EyeGazeData    gaze;       // 注视点数据
} GSXR_EyeTrackingData;

/**
 *  说明：
 *      获取支持的眼球数据类型
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] factors GSXR_EyeFactor 之位掩码, 输出所有支持的眼球数据类型
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_眼球追踪类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSupportedEyeFactors(
    GSXR_XrFeature      feature,
    GSXR_Flags64*       factors);

/**
 *  说明：
 *      设置 XR 应用所有需要使用的眼球数据类型
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] request GSXR_EyeFactor 之位掩码, 指定欲使用的眼球数据类型
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      XR 应用需优先调用 GSXR_SetEyeFactorRequest 完成眼球数据请求设置后, 始能获取眼球数据
 *  所属：
 *      GSXR_API_眼球追踪类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetEyeFactorRequest(
    GSXR_XrFeature      feature,
    GSXR_Flags64        request);

/**
 *  说明：
 *      获取指定时间之眼球追踪数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] specifiedTime 指定时间
 *      [out] data 指向 GSXR_EyeTrackingData 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_眼球追踪类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetEyeTrackingData(
    GSXR_XrFeature              feature,
    GSXR_Time                   specifiedTime,
    GSXR_EyeTrackingData*       data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_EYE_TRACKING_H_ */
