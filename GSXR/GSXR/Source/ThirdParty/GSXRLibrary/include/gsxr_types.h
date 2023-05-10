#ifndef GSXR_TYPES_H_
#define GSXR_TYPES_H_

/**
 *  Declare GSXR Copyright here.
 */
 
#include "gsxr_stdinc.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------
// GSXR_API_类型定义类
// ------------------------------------

#define GSXR_MAKE_VERSION(major, minor, patch) \
    ((((major) & 0xffffULL) << 48) | (((minor) & 0xffffULL) << 32) | ((patch) & 0xffffffffULL))

#define GSXR_VERSION_MAJOR(version) (uint16_t)(((uint64_t)(version) >> 48) & 0xffffULL)
#define GSXR_VERSION_MINOR(version) (uint16_t)(((uint64_t)(version) >> 32) & 0xffffULL)
#define GSXR_VERSION_PATCH(version) (uint32_t)((uint64_t)(version) & 0xffffffffULL)

#define GSXR_CURRENT_API_VERSION GSXR_MAKE_VERSION(0, 90, 0)

// bits 63-48: 主版本号
// bits 47-32: 次版本号
// bits 31-0:  修订号
typedef uint64_t    GSXR_Version;


typedef uint32_t    GSXR_Bool32;
typedef uint64_t    GSXR_Flags64;
typedef int64_t     GSXR_Time;
typedef int64_t     GSXR_Duration;

#define GSXR_TRUE   1
#define GSXR_FALSE  0

#define GSXR_NO_DURATION        0
#define GSXR_INFINITE_DURATION  0x7fffffffffffffffLL

#define GSXR_COMMON_STRING_MAX_SIZE 128

#define GSXR_NULL_HANDLE        0
#define GSXR_NULL_DISPLAY_ID    0

#define GSXR_IS_COMMON_EVENT(event) (((event) >= 1000) && ((event) < 2000))
#define GSXR_IS_DEVICE_STATE_EVENT(event) (((event) >= 2000) && ((event) < 3000))
#define GSXR_IS_DEVICE_INPUT_EVENT(event) (((event) >= 3000) && ((event) < 4000))
#define GSXR_IS_RENDERER_STATE_EVENT(event) (((event) >= 4000) && ((event) < 5000))
#define GSXR_IS_FEATURE_STATE_EVENT(event) (((event) >= 5000) && ((event) < 6000))

#if !defined(GSXR_DEFINE_HANDLE)
    #define GSXR_DEFINE_HANDLE(object) typedef void* object;
#endif

#if !defined(GSXR_DEFINE_ATOM)
    #define GSXR_DEFINE_ATOM(object) typedef uint64_t object;
#endif

GSXR_DEFINE_HANDLE(GSXR_Runtime)    // GSXR Runtime 句柄
GSXR_DEFINE_ATOM(GSXR_DisplayId)    // GSXR 显示标识号
// 枚举函数返回码
typedef enum GSXR_Result {
    GSXR_Result_Success                         = 0,    // 函数调用成功
    GSXR_Result_Event_Unavailable               = 1,    // 事件队列中已无事件
    GSXR_Result_Timeout_Expired                 = 2,    // 等待超时
    GSXR_Result_Device_Connection_Pending       = 3,    // 直至设备连线

    GSXR_Error_Operation_Failed                 = -1,   // 函数运行失败
    GSXR_Error_API_Unsupported                  = -2,   // Runtime 未支持此 API
    GSXR_Error_ApplicationType_Unsupported      = -3,   // 设备未支持此应用类型
    GSXR_Error_Function_Unsupported             = -4,   // 设备未支持此功能
    GSXR_Error_Feature_Unsupported              = -5,   // 设备未支持此特性
    GSXR_Error_Device_Disconnected              = -6,   // 设备未连接
    GSXR_Error_Handle_Invalid                   = -7,   // 无效的输入句柄
    GSXR_Error_Argument_Invalid                 = -8,   // 无效的输入参数
    GSXR_Error_Call_Flow_Invalid                = -9,   // 函数调用顺序错误
    GSXR_Error_Out_Of_Memory                    = -10,  // 内存溢出
    GSXR_Error_Runtime_Unavailable              = -11,  // Runtime 未处于正确工作状态
    GSXR_Error_Renderer_Unavailable             = -12,  // 渲染器未处于正确工作状态
    GSXR_Error_Feature_Unavailable              = -13,  // 特性未处于正确工作状态
} GSXR_Result;

// 枚举事件类型
typedef enum GSXR_EventType {
    /** Common */
    GSXR_EventType_Common_SpaceOrigin_Located               = 1000, // 应用空间原点设置完成
    GSXR_EventType_Common_SpaceOrigin_Relocalized           = 1001, // 系统重置空间原点
    GSXR_EventType_Common_SpaceBounds_Changed               = 1002, // 系统更动空间边界

    /** Device State */
    GSXR_EventType_DeviceState_Connected                    = 2000, // 设备连接
    GSXR_EventType_DeviceState_Disconnected                 = 2001, // 设备断开
    GSXR_EventType_DeviceState_InputMapping_Paired          = 2002, // 输入部件配对完成
    GSXR_EventType_DeviceState_TrackingMode_Changed         = 2003, // 设备跟踪模式变换

    /** Device Input */
    GSXR_EventType_DeviceInput_Clicked                      = 3000, // 起始点击
    GSXR_EventType_DeviceInput_Unclicked                    = 3001, // 结束点击
    GSXR_EventType_DeviceInput_Touched                      = 3002, // 起始触摸
    GSXR_EventType_DeviceInput_Untouched                    = 3003, // 结束触摸
    GSXR_EventType_DeviceInput_LeftToRight_Swiped           = 3004, // 左至右滑动
    GSXR_EventType_DeviceInput_RightToLeft_Swiped           = 3005, // 右至左滑动
    GSXR_EventType_DeviceInput_TopToBottom_Swiped           = 3006, // 上至下滑动
    GSXR_EventType_DeviceInput_BottomToTop_Swiped           = 3007, // 下至上滑动

    /** Renderer State */
    GSXR_EventType_RendererState_Renderer_Created           = 4000, // 渲染器创建
    GSXR_EventType_RendererState_Renderer_Destroyed         = 4001, // 渲染器销毁
    GSXR_EventType_RendererState_TextureQueue_Created       = 4002, // 纹理队列创建
    GSXR_EventType_RendererState_TextureQueue_Destroyed     = 4003, // 纹理队列销毁
    GSXR_EventType_RendererState_Looper_Available           = 4004, // 渲染循环状态可用
    GSXR_EventType_RendererState_Looper_Unavailable         = 4005, // 渲染循环状态不可用
    GSXR_EventType_RendererState_Looper_Starting            = 4006, // 渲染循环起始中
    GSXR_EventType_RendererState_Looper_Stopping            = 4007, // 渲染循环结束中
    GSXR_EventType_RendererState_Looper_Ready               = 4008, // 渲染循环已备妥
    GSXR_EventType_RendererState_Looper_Unready             = 4009, // 渲染循环未备妥
    GSXR_EventType_RendererState_Looper_Focused             = 4010, // 渲染循环获取输入焦点
    GSXR_EventType_RendererState_Looper_Unfocused           = 4011, // 渲染循环失去输入焦点
    GSXR_EventType_RendererState_ViewSet_Changed            = 4012, // 视图集内容变更
    GSXR_EventType_RendererState_ViewSet_Added              = 4013, // 视图集添加
    GSXR_EventType_RendererState_ViewSet_Removed            = 4014, // 视图集删除

    /** Feature State */
    GSXR_EventType_FeatureState_Unavailable                 = 5000, // 特性状态不可用
    GSXR_EventType_FeatureState_Available                   = 5001, // 特性状态可用
    GSXR_EventType_FeatureState_Starting                    = 5002, // 特性状态起始中
    GSXR_EventType_FeatureState_Working                     = 5003, // 特性状态运作中
    GSXR_EventType_FeatureState_Stopping                    = 5004, // 特性状态结束中
} GSXR_EventType;

// 二维矢量结构体
typedef struct GSXR_Vector2f {
    float x;
    float y;
} GSXR_Vector2f;

// 三维矢量结构体
typedef struct GSXR_Vector3f {
    float x;
    float y;
    float z;
} GSXR_Vector3f;

// 四元数结构体
typedef struct GSXR_Quaternionf {
    float x;
    float y;
    float z;
    float w;
} GSXR_Quaternionf;

// 位姿结构体
typedef struct GSXR_Posef {
    GSXR_Vector3f       position;       // 位置
    GSXR_Quaternionf    orientation;    // 方向
} GSXR_Posef;

// 速度结构体
typedef struct GSXR_Velocityf {
    GSXR_Vector3f   linearVelocity;     // 线速度
    GSXR_Vector3f   angularVelocity;    // 角速度
} GSXR_Velocityf;

// 空间姿态结构体
typedef struct GSXR_PoseState {
    GSXR_Bool32      isPoseValid;       // GSXR_TRUE 表示位姿有效, GSXR_FALSE 表示位姿无效
    GSXR_Posef       pose;              // 位姿结构体
    GSXR_Bool32      isVelocityValid;   // GSXR_TRUE 表示速度有效, GSXR_FALSE 表示速度无效
    GSXR_Velocityf   velocity;          // 速度结构体
} GSXR_PoseState;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GSXR_TYPES_H_ */
