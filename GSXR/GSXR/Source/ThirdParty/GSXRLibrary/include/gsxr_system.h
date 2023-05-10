#ifndef GSXR_SYSTEM_H_
#define GSXR_SYSTEM_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_types.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------
// GSXR_API_系統性能类
// ------------------------------------

// 枚举系统性能等级
typedef enum GSXR_PerformanceLevel {
    GSXR_PerformanceLevel_System    = 0,    // 系统预设等级
    GSXR_PerformanceLevel_High      = 1,    // 高性能等级
    GSXR_PerformanceLevel_Medium    = 2,    // 中性能等级
    GSXR_PerformanceLevel_Low       = 3,    // 低性能等级
} GSXR_PerformanceLevel;

/**
 *  说明：
 *      设置 CPU, GPU 性能等级
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] cpuPerfLevel 设置 CPU 性能等级
 *       [in] gpuPerfLevel 设置 GPU 性能等级
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_系統性能类
 */
extern GSXR_EXPORT bool GSXR_SetPerformanceLevels(
    GSXR_Runtime                runtime,
    GSXR_PerformanceLevel       cpuPerfLevel,
    GSXR_PerformanceLevel       gpuPerfLevel);

// ------------------------------------
// GSXR_API_系統功能设置类
// ------------------------------------
// 枚举系统性能等级
typedef enum GSXR_XrSystemSettingType {
    GSXR_XrSystemSettingType_Wifi                 = 1,   // Wifi设置
    GSXR_XrSystemSettingType_Bluetooth            = 2,   // 蓝牙设置
} GSXR_XrSystemSettingType;

/**
 *  说明：
 *      系统设置
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] settingType 设置系统功能类型
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_系统设置类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetSystemSetting(
        GSXR_Runtime                runtime,
        GSXR_XrSystemSettingType    settingType);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_SYSTEM_H_ */
