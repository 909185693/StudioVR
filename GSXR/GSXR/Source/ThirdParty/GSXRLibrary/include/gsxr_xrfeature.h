#ifndef GSXR_XRFEATURE_H_
#define GSXR_XRFEATURE_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_types.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

GSXR_DEFINE_HANDLE(GSXR_XrFeature)   // XR 特性句柄

// ------------------------------------
// GSXR_API_延伸特性类
// ------------------------------------

// 枚举特性类型
typedef enum GSXR_XrFeatureType {
    GSXR_XrFeatureType_EyeTracking          = 0x00000001,   // 眼球追踪
    GSXR_XrFeatureType_HandTracking         = 0x00000002,   // 手部追踪
    GSXR_XrFeatureType_VoiceCommand         = 0x00000010,   // 语音命令
    GSXR_XrFeatureType_ControllerModel      = 0x00000100,   // 手柄模型
    GSXR_XrFeatureType_PointCloud           = 0x00010000,   // 点云数据
    GSXR_XrFeatureType_PlaneDetection       = 0x00020000,   // 平面侦测
    GSXR_XrFeatureType_ImageRecognition     = 0x00040000,   // 图像识别
    GSXR_XrFeatureType_FaceRecognition      = 0x00080000,   // 脸部识别
} GSXR_XrFeatureType;

// 特性状态事件结构体
typedef struct GSXR_XrFeatureStateEventData {
    GSXR_XrFeatureType  featureType;        // 特性类型
} GSXR_XrFeatureStateEventData;

/**
 *  说明：
 *      获取当前支持的所有特性类型
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] featureTypeFlags GSXR_XrFeatureType 之位掩码, 输出可支持的所有特性类型
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_延伸特性类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSupportedXrFeatures(
    GSXR_Runtime        runtime,
    GSXR_Flags64*       featureTypeFlags);

/**
 *  说明：
 *      获取特性可用状态
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] featureType 指定特性类型
 *      [out] available 输出 GSXR_TRUE 表示特性可用, GSXR_FALSE 表示特性不可用
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_延伸特性类
 */
extern GSXR_EXPORT GSXR_Result GSXR_IsXrFeatureAvailable(
    GSXR_Runtime        runtime,
    GSXR_XrFeatureType  featureType,
    GSXR_Bool32*        available);

/**
 *  说明：
 *      获取特性之功能选项
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] featureType 指定特性类型
 *      [out] recommendedOptions 各特性之功能选项位掩码, 输出系統建議的功能选项
 *      [out] supportedOptions 各特性之功能选项位掩码, 输出可支持的所有功能选项
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      各特性之功能选项定义于该功能之头文件中
 *  所属：
 *      GSXR_API_延伸特性类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetXrFeatureOptions(
    GSXR_Runtime            runtime,
    GSXR_XrFeatureType      featureType,
    GSXR_Flags64*           recommendedOptions,
    GSXR_Flags64*           supportedOptions);

// 特性起始资讯结构体
typedef struct GSXR_XrFeatureStartInfo {
    void*                   next;           // 预留指针
    GSXR_XrFeatureType      featureType;    // 指定特性类型
    GSXR_Flags64            options;        // 设置启用的特性功能选项
} GSXR_XrFeatureStartInfo;

/**
 *  说明：
 *      起始特性运行
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] startInfo 指向 GSXR_XrFeatureStartInfo 结构体, 内含特性起始资讯
 *      [out] feature 特性起始成功输出一有效句柄, 起始失败输出 GSXR_NULL_HANDLE
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Out_Of_Memory
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      特性状态需处于 GSXR_EventType_FeatureState_Available 方能起始特性运行
 *  所属：
 *      GSXR_API_延伸特性类
 */
extern GSXR_EXPORT GSXR_Result GSXR_StartXrFeature(
    GSXR_Runtime                    runtime,
    const GSXR_XrFeatureStartInfo*  startInfo,
    GSXR_XrFeature*                 feature);


/**
 *  说明：
 *      停止特性运行
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_延伸特性类
 */
extern GSXR_EXPORT GSXR_Result GSXR_StopXrFeature(
    GSXR_XrFeature        feature);


// 特性属性结构体
typedef struct GSXR_XrFeatureProperties {
    void*   next;                                           // 预留指针
    char    serialNumber[GSXR_COMMON_STRING_MAX_SIZE];      // 特性设备 SN 串号
    char    firmwareVersion[GSXR_COMMON_STRING_MAX_SIZE];   // 特性设备 firmware 版本号
    char    vendorName[GSXR_COMMON_STRING_MAX_SIZE];        // 特性厂家名称
    char    algorithmVersion[GSXR_COMMON_STRING_MAX_SIZE];  // 特性算法版本号
    char    algorithmName[GSXR_COMMON_STRING_MAX_SIZE];     // 特性算法名称
} GSXR_XrFeatureProperties;

/**
 *  说明：
 *      获取特性属性
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] properties 指向 GSXR_XrFeatureProperties 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_延伸特性类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetXrFeatureProperties(
    GSXR_XrFeature                feature,
    GSXR_XrFeatureProperties*     properties);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_XRFEATURE_H_ */
