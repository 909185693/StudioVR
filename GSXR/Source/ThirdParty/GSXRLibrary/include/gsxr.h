#ifndef GSXR_H_
#define GSXR_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_types.h"
#include "gsxr_device.h"
#include "gsxr_renderer.h"
#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------
// GSXR_API_通用基础类
// ------------------------------------

/**
 *  说明：
 *      转换 GSXR_Result 返回码为字符串
 *  参数：
 *       [in] result GSXR_Result 返回码
 *      [out] string 输出转换之字符串
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_API_Unsupported
 *      GSXR_Error_Argument_Invalid
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_通用基础类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ResultToString(
    GSXR_Result     result,
    char            string[GSXR_COMMON_STRING_MAX_SIZE]);

// 枚举应用类型
typedef enum GSXR_ApplicationType {
    GSXR_ApplicationType_VR     = 1,        // VR 应用
    GSXR_ApplicationType_AR     = 2,        // AR 应用
} GSXR_ApplicationType;
    
// Runtime 初始资讯结构体
typedef struct GSXR_RuntimeCreateInfo {
    void*                   next;           // 预留指针
    GSXR_ApplicationType    appType;        // XR 应用类型
    const void*             systemBinding;  // 操作系统的特化结构
                                            // Note. 在移动设备系统, systemBinding 指向 GSXR_AndrosSystemBinding
} GSXR_RuntimeCreateInfo;

/**
 *  说明：
 *      创建并初始 GSXR Runtime 实例, 始能XR Runtime运行平台
 *  参数：
 *       [in] createInfo 指向 GSXR_RuntimeCreateInfo 之结构体, 内含 Runtime 初始资讯
 *      [out] runtime 指向 GSXR_Runtime 句柄之指针, 实例创建成功输出一有效句柄, 创建失败输出 GSXR_NULL_HANDLE
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_ApplicationType_Unsupported
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Out_Of_Memory
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_通用基础类
 */
extern GSXR_EXPORT GSXR_Result GSXR_CreateRuntime(
    const GSXR_RuntimeCreateInfo*   createInfo,
    GSXR_Runtime*                   runtime);

/**
 *  说明：
 *      终止并销毁 GSXR Runtime 实例
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_通用基础类
 */
extern GSXR_EXPORT GSXR_Result GSXR_DestroyRuntime(
    GSXR_Runtime        runtime);


// Runtime 属性结构体
typedef struct GSXR_RuntimeProperties {
    void*           next;                                       // 预留指针
    GSXR_Version    gsxrVersion;                                // Runtime 实现的 GSXR 规范版本号
    GSXR_Version    runtimeVersion;                             // Runtime 版本号
    char            runtimeName[GSXR_COMMON_STRING_MAX_SIZE];   // Runtime 实现者名称
} GSXR_RuntimeProperties;

/**
 *  说明：
 *      获取 GSXR Runtime 属性
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] runtimeProperties 指向 GSXR_RuntimeProperties 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_API_Unsupported
 *      GSXR_Error_Handle_Invalid
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_通用基础类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetRuntimeProperties(
    GSXR_Runtime                runtime,
    GSXR_RuntimeProperties*     runtimeProperties);


typedef union {
    GSXR_DeviceStateEventData          gsxr_devicesatate_eventdata;
    GSXR_DeviceInputEventData          gsxr_deviceinput_eventdata;
    GSXR_RendererStateEventData        gsxr_rendererstate_eventdata;
    GSXR_XrFeatureStateEventData       gsxr_xrfeaturestate_eventdata;
}GSXR_EventData;



#define GSXR_EVENT_BUFFER_MAX_SIZE 512

// GSXR 事件结构体
typedef struct GSXR_Event {
    void*               next;                                   // 预留指针
    GSXR_EventType      type;                                   // 事件类型
    GSXR_Time           timestamp;                              // 事件发生时间戳
    uint8_t             buffer[GSXR_EVENT_BUFFER_MAX_SIZE];     // 预留足够长度的事件数据缓存
    GSXR_EventData      gsxrEventData;
} GSXR_Event;

/**
 *  说明：
 *      从事件队列中获取 GSXR 事件, 返回 GSXR_Result_Success 代表队列中尚有事件, 需再次调用函数获取下一事件, 
 *      直至返回 GSXR_Result_Event_Unavailable 代表队列中已无新事件
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] event 指向 GSXR_Event 结构体, 输出指针需依事件类型转换至该事件之对应结构体指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Result_Event_Unavailable
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_通用基础类
 */
extern GSXR_EXPORT GSXR_Result GSXR_PollEvent(
    GSXR_Runtime        runtime,
    GSXR_Event*         event);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_H_ */
