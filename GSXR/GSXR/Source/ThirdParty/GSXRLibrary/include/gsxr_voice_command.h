#ifndef GSXR_VOICE_COMMAND_H_
#define GSXR_VOICE_COMMAND_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举语音命令特性之功能选项
typedef enum GSXR_VoiceCommandOptions {
    GSXR_VoiceCommandOptions_Reserved   = 0xFFFFFFFF,
} GSXR_VoiceCommandOptions;

// ------------------------------------
// GSXR_API_语音命令类
// ------------------------------------

/**
 *  说明：
 *      输入语音命令
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] timeout 语音输入的时限值, 单位为 naroseconds
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
 *      GSXR_API_语音命令类
 */
extern GSXR_EXPORT GSXR_Result GSXR_InputVoiceCommand(
    GSXR_XrFeature          feature,
    GSXR_Duration           timeout);


#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_VOICE_COMMAND_H_ */
