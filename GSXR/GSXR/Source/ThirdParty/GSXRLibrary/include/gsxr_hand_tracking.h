#ifndef GSXR_HAND_TRACKING_H_
#define GSXR_HAND_TRACKING_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举手部追踪特性之功能选项
typedef enum GSXR_HandTrackingOptions {
    GSXR_HandTrackingOptions_Hand_Left              = 0x00000001,   // 左手追踪
    GSXR_HandTrackingOptions_Hand_Right             = 0x00000002,   // 右手追踪

    GSXR_HandTrackingOptions_Gesture_Recognition    = 0x00010000,   // 手势识别
    GSXR_HandTrackingOptions_Skeleton_Motion        = 0x00020000,   // 骨骼节点追踪
    GSXR_HandTrackingOptions_Pinch_Interaction      = 0x00040000,   // 捏合姿态追踪
} GSXR_HandTrackingOptions;

// ------------------------------------
// GSXR_API_手势识别类
// ------------------------------------

// 枚举手势类型
typedef enum GSXR_HandGestureType {
    GSXR_HandGestureType_Unavailable = 0,           // 未起始手势识别功能
    GSXR_HandGestureType_Unknown     = 0x00000001,  // 未知手势
    GSXR_HandGestureType_Fist        = 0x00000002,  // 握拳
    GSXR_HandGestureType_Five        = 0x00000004,  // 张开
    GSXR_HandGestureType_OK          = 0x00000008,  // OK
    GSXR_HandGestureType_ThumbUp     = 0x00000010,  // 拇指朝上
    GSXR_HandGestureType_IndexUp     = 0x00000020,  // 食指朝上
} GSXR_HandGestureType;

// 手势数据结构体
typedef struct GSXR_HandGestureData {
    void*                   next;       // 预留指针
    GSXR_HandGestureType    left;       // 左手手势
    GSXR_HandGestureType    right;      // 右手手势
} GSXR_HandGestureData;

/**
 *  说明：
 *      获取可识别的手势类型
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] gestures GSXR_HandGestureType 之位掩码, 输出所有支持的手势类型
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
 *      GSXR_API_手势识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSupportedHandGestures(
    GSXR_XrFeature      feature,
    GSXR_Flags64*       gestures);

/**
 *  说明：
 *      设置 XR 应用所有需要使用的手势类型
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] request GSXR_HandGestureType 之位掩码, 指定欲使用的手势类型
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
 *      XR 应用需优先调用 GSXR_SetHandGestureRequest 完成手势请求设置后, 始能获取手势数据
 *  所属：
 *      GSXR_API_手势识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetHandGestureRequest(
    GSXR_XrFeature      feature,
    GSXR_Flags64        request);

/**
 *  说明：
 *      获取手势数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] data 指向 GSXR_HandGestureData 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      若识别为双手手势, left 与 right 指定同一双手手势
 *  所属：
 *      GSXR_API_手势识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetHandGestureData(
    GSXR_XrFeature              feature,
    GSXR_HandGestureData*       data);

// 枚举左右手
typedef enum GSXR_Hand {
    GSXR_Hand_Left      = 0,    // 左手
    GSXR_Hand_Right     = 1,    // 右手
    GSXR_Hand_Both      = 2,    // 双手
} GSXR_Hand;

// 手势识别事件结构体
typedef struct GSXR_HandGestureEvent {
    void*                   next;       // 预留指针
    GSXR_Time               timestamp;  // 事件发生时间戳
    GSXR_Hand               hand;       // 左,右,或双手
    GSXR_HandGestureType    gesture;    // 手势类型
} GSXR_HandGestureEvent;


/**
 *  说明：
 *      手势识别事件回调函数, 当算法识别出应用需求之手势时回调
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] event 指向 GSXR_HandGestureEvent 结构体
 *       [in] cookie 函数回调时带回的预备数值
 *  返回值：
 *      无
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_手势识别类
 */
typedef void (*GSXR_HandGestureEventCallback)(
    GSXR_XrFeature                  feature,
    const GSXR_HandGestureEvent*    event,
    void*                           cookie);

/**
 *  说明：
 *      注册手势识别事件回调函数
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] callback 手势识别事件回调函数
 *       [in] cookie 函数回调时带回的预备数值
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
 *      无
 *  所属：
 *      GSXR_API_手势识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_RegisterHandGestureEventCallback(
    GSXR_XrFeature                  feature,
    GSXR_HandGestureEventCallback   callback,
    void*                           cookie);

// ------------------------------------
// GSXR_API_手部追踪类
// ------------------------------------


#define GSXR_HAND_JOINT_COUNT 26

// 枚举手部节点
typedef enum GSXR_HandJoint {
    GSXR_HandJoint_Palm                 = 0,
    GSXR_HandJoint_Wrist                = 1,
    GSXR_HandJoint_Thumb_Metacarpal     = 2,
    GSXR_HandJoint_Thumb_Proximal       = 3,
    GSXR_HandJoint_Thumb_Distal         = 4,
    GSXR_HandJoint_Thumb_Tip            = 5,
    GSXR_HandJoint_Index_Metacarpal     = 6,
    GSXR_HandJoint_Index_Proximal       = 7,
    GSXR_HandJoint_Index_Intermediate   = 8,
    GSXR_HandJoint_Index_Distal         = 9,
    GSXR_HandJoint_Index_Tip            = 10,
    GSXR_HandJoint_Middle_Metacarpal    = 11,
    GSXR_HandJoint_Middle_Proximal      = 12,
    GSXR_HandJoint_Middle_Intermediate  = 13,
    GSXR_HandJoint_Middle_Distal        = 14,
    GSXR_HandJoint_Middle_Tip           = 15,
    GSXR_HandJoint_Ring_Metacarpal      = 16,
    GSXR_HandJoint_Ring_Proximal        = 17,
    GSXR_HandJoint_Ring_Intermediate    = 18,
    GSXR_HandJoint_Ring_Distal          = 19,
    GSXR_HandJoint_Ring_Tip             = 20,
    GSXR_HandJoint_Little_Metacarpal    = 21,
    GSXR_HandJoint_Little_Proximal      = 22,
    GSXR_HandJoint_Little_Intermediate  = 23,
    GSXR_HandJoint_Little_Distal        = 24,
    GSXR_HandJoint_Little_Tip           = 25,
} GSXR_HandJoint;

// 枚举手部节点空间追踪能力
typedef enum GSXR_HandJointValidity {
    GSXR_HandJointValidity_Pose_Position    = 0x00000001,
    GSXR_HandJointValidity_Pose_Orientation = 0x00000002,
    
    GSXR_HandJointValidity_Velocity_Linear  = 0x00010000,
    GSXR_HandJointValidity_Velocity_Angular = 0x00020000,
} GSXR_HandJointValidity;

// 手部骨骼节点数据结构体
typedef struct GSXR_HandSkeletonData {
    void*               next;               // 预留指针
    GSXR_Bool32         onTracking;         // GSXR_TRUE 表示手部节点追踪中, GSXR_FALSE 表示未起始手部节点追踪
    GSXR_Bool32         isValid;            // GSXR_TRUE 表示手部节点数据有效, GSXR_FALSE 表示手部节点数据无效
    uint32_t            jointCount;         // 手部节点数目, GSXR_HAND_JOINT_COUNT
    GSXR_PoseState*     jointsPoseState;    // 指向 GSXR_PoseState 结构体之数组, 输出手部节点姿态
} GSXR_HandSkeletonData;

/**
 *  说明：
 *      获取可被追踪的手部节点
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] joints (1 << GSXR_HandJoint) 之位掩码, 输出算法支持追踪的手部节点
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
 *      GSXR_API_手部追踪类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetTrackedHandJoints(
    GSXR_XrFeature      feature,
    GSXR_Flags64*       joints);

/**
 *  说明：
 *      获取手部节点空间追踪能力
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] validity GSXR_HandJointValidity 之位掩码, 输出手部节点的追踪能力
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
 *      GSXR_API_手部追踪类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetHandJointValidity(
    GSXR_XrFeature      feature,
    GSXR_Flags64*       validity);

// 枚举手指
typedef enum GSXR_HandFinger {
    GSXR_HandFinger_Thumb   = 1,
    GSXR_HandFinger_Index   = 2,
    GSXR_HandFinger_Middle  = 3,
    GSXR_HandFinger_Ring    = 4,
    GSXR_HandFinger_Little  = 5,
} GSXR_HandFinger;

// 手部捏合数据结构体
typedef struct GSXR_HandPinchData {
    void*               next;           // 预留指针
    GSXR_Bool32         onTracking;     // GSXR_TRUE 表示手部捏合追踪中, GSXR_FALSE 表示未起始手部捏合追踪
    GSXR_Bool32         isValid;        // GSXR_TRUE 表示手部捏合数据有效, GSXR_FALSE 表示手部捏合数据无效
    GSXR_HandFinger     finger;         // 往拇指捏合的手指
    float               strength;       // 捏合两指的临近度, 数据范围为 0 - 1, 1 表示两指相互碰触捏合
    GSXR_PoseState      pose;           // 捏合姿态
} GSXR_HandPinchData;

// 手部追踪数据结构体
typedef struct GSXR_HandTrackingData {
    void*                   next;           // 预留指针
    GSXR_HandSkeletonData   leftHand;       // 左手骨骼数据
    GSXR_HandSkeletonData   rightHand;      // 右手骨骼数据
    GSXR_HandPinchData      leftPinch;      // 左手捏合数据
    GSXR_HandPinchData      rightPinch;     // 右手捏合数据
} GSXR_HandTrackingData;

/**
 *  说明：
 *      获取指定时间之手部追踪数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] specifiedTime 指定时间
 *      [out] data 指向 GSXR_HandTrackingData 结构体
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
 *      GSXR_API_手部追踪类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetHandTrackingData(
    GSXR_XrFeature              feature,
    GSXR_Time                   specifiedTime,
    GSXR_HandTrackingData*      data);


#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_HAND_TRACKING_H_ */
