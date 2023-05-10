#ifndef GSXR_DEVICE_H_
#define GSXR_DEVICE_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_types.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif
// 枚举设备类型
typedef enum GSXR_DeviceType  {
    GSXR_DeviceType_HMD_AR              = 1,   // AR 头戴式显示器
    GSXR_DeviceType_HMD_VR              = 2,   // VR 头戴式显示器
    GSXR_DeviceType_Controller_Right    = 3,   // 右手手柄控制器
    GSXR_DeviceType_Controller_Left     = 4,   // 左手手柄控制器
    GSXR_DeviceType_BaseStation         = 5,   // 基站模组
    GSXR_DeviceType_LeftFoot            = 6,   // 左脚
    GSXR_DeviceType_RightFoot           = 7,   // 右脚
    GSXR_DeviceType_LeftShoulder        = 8,   // 左肩膀
    GSXR_DeviceType_RightShoulder       = 9,   // 右肩膀
    GSXR_DeviceType_Waist               = 10,   // 腰部
    GSXR_DeviceType_Chest               = 11,   // 胸部
    GSXR_DeviceType_Threadmil           = 12,   // ThreadMil
    GSXR_DeviceType_GamePad             = 13,   // GamePad
    GSXR_DeviceType_Tracker             = 64,   // 独立跟踪模组
} GSXR_DeviceType;
//触摸板控制器按键ID，列如HTC控制器
typedef enum GSXR_Joystick_InputId{
    GSXR_Joystick_InputId_A                = 1,
    GSXR_Joystick_InputId_B                = 2,
    GSXR_Joystick_InputId_X                = 3,
    GSXR_Joystick_InputId_Y                = 4,
    GSXR_Joystick_InputId_System           = 5,
    GSXR_Joystick_InputId_Menu             = 6,
    GSXR_Joystick_InputId_Grip             = 7,
    GSXR_Joystick_InputId_Trigger          = 8,
    GSXR_Joystick_InputId_Thumbstick       = 9,
    GSXR_Joystick_InputId_Dpad_Left        = 10,
    GSXR_Joystick_InputId_Dpad_Up          = 11,
    GSXR_Joystick_InputId_Dpad_Right       = 12,
    GSXR_Joystick_InputId_Dpad_Down        = 13,
}GSXR_Joystick_InputId;
//摇杆控制器按键ID，列如OC Quest控制器
typedef enum GSXR_Touch_InputId{
    GSXR_Touch_InputId_System        = 1,
    GSXR_Touch_InputId_Menu          = 2,
    GSXR_Touch_InputId_Grip          = 3,
    GSXR_Touch_InputId_Trigger       = 4,
    GSXR_Touch_InputId_TrackPad      = 5,
    GSXR_Touch_InputId_Dpad_Left     = 6,
    GSXR_Touch_InputId_Dpad_Up       = 7,
    GSXR_Touch_InputId_Dpad_Right    = 8,
    GSXR_Touch_InputId_Dpad_Down     = 9,
}GSXR_Touch_InputId;
// Gamepad控制器按键ID
typedef enum GSXR_Gamepad_InputId{
    GSXR_Gamepad_InputId_A                        = 1,
    GSXR_Gamepad_InputId_B                        = 2,
    GSXR_Gamepad_InputId_X                        = 3,
    GSXR_Gamepad_InputId_Y                        = 4,
    GSXR_Gamepad_InputId_Left_Thumbstick          = 5,
    GSXR_Gamepad_InputId_Right_Thumbstick         = 6,
    GSXR_Gamepad_InputId_Left_Thumbstick_Up       = 7,
    GSXR_Gamepad_InputId_Left_Thumbstick_Down     = 8,
    GSXR_Gamepad_InputId_Left_Thumbstick_Left     = 9,
    GSXR_Gamepad_InputId_Left_Thumbstick_Right    = 10,
    GSXR_Gamepad_InputId_Right_Thumbstick_Up      = 11,
    GSXR_Gamepad_InputId_Right_Thumbstick_Dwon    = 12,
    GSXR_Gamepad_InputId_Right_Thumbstick_Left    = 13,
    GSXR_Gamepad_InputId_Right_Thumbstick_Right   = 14,
    GSXR_Gamepad_InputId_Dpad_Up                  = 15,
    GSXR_Gamepad_InputId_Dpad_Down                = 16,
    GSXR_Gamepad_InputId_Dpad_Left                = 17,
    GSXR_Gamepad_InputId_Dpad_Right               = 18,
    GSXR_Gamepad_InputId_Left_Trigger             = 19,
    GSXR_Gamepad_InputId_Left_Shoulder            = 20,
    GSXR_Gamepad_InputId_Right_Trigger            = 21,
    GSXR_Gamepad_InputId_Right_Shoulder           = 22,
}GSXR_Gamepad_InputId;

typedef enum GSXR_Hmd_InputId{
    GSXR_Hmd_InputId_Enter                        = 1,
    GSXR_Hmd_InputId_Back                         = 2,
    GSXR_Hmd_InputId_Volume_Up                    = 3,
    GSXR_Hmd_InputId_Volume_Down                  = 4,
    GSXR_Hmd_InputId_Home                         = 5,
}GSXR_Hmd_InputId;

inline uint64_t GSXRMaskFromId( int id ) { return 1ull << id; }

//设备输入类型
typedef enum GSXR_Device_InputType{
    GSXR_NO_Supported_InputType                  = 1,
    GSXR_Touch                                   = 2,
    GSXR_Joystick                                = 3,
    GSXR_GamePad                                 = 4,
    GSXR_Hmd                                     = 5,
}GSXR_Device_InputType;

// 枚举InputID类型
typedef enum GSXR_InputId_InputType {
    GSXR_InputId_InputType_Click        = 0x00000001,   // 点击类
    GSXR_InputId_InputType_Touch        = 0x00000002,   // 触摸类
    GSXR_InputId_InputType_Analog_1D    = 0x00000004,   // 类比类 - 一维数据
    GSXR_InputId_InputType_Analog_2D    = 0x00000008,   // 类比类 - 二维数据
} GSXR_InputId_InputType;

// 设备状态事件结构体
typedef struct GSXR_DeviceStateEventData {
    GSXR_DeviceType     deviceType;     // 设备类型
} GSXR_DeviceStateEventData;

// 设备输入事件结构体
typedef struct GSXR_DeviceInputEventData {
    GSXR_DeviceType     deviceType;     // 设备类型
    int                 inputId;        // 设备输入部件标识号
} GSXR_DeviceInputEventData;

// ------------------------------------
// GSXR_API_设备输入类
// ------------------------------------

/**
 *  说明：
 *      获取当前支持的所有设备类型
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] deviceTypeFlags 指向 GSXR_Flags64 数值之指针, 数值为 GSXR_DeviceType 之位掩码, 输出可支持的所有设备类型
 * 
 *  案例：(支持头部控件、左手控制器、右手控制器)
 *      *deviceTypeFlags = GSXRMaskFromId(GSXR_DeviceType_HMD_VR)+GSXRMaskFromId(GSXR_DeviceType_Controller_Left)+GSXRMaskFromId(GSXR_DeviceType_Controller_Right)
 * 
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSupportedDevices(
    GSXR_Runtime        runtime,
    GSXR_Flags64*       deviceTypeFlags);
    
/**
 *  说明：
 *      获取当前主手柄，会被用来发射UI交互射线
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] deviceTypeFlags 指向 GSXR_Flags64 数值之指针, 数值为GSXR_DeviceType, 输出主手柄设备类型
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetPrimaryDeviceType(
    GSXR_Runtime        runtime,
    GSXR_Flags64*       deviceTypeFlags);

    /**
 *  说明：
 *      设置当前主手柄设备类型
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType数值为GSXR_DeviceType, 指定主手柄设备类型
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetPrimaryDeviceType(
    GSXR_Runtime        runtime,
    GSXR_DeviceType     deviceType);

/**
 *  说明：
 *      获取当前设备支持的输入类型
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型  
 *      [out] deviceInputTypeFlag 指向 GSXR_Flags64 数值之指针, 数值为 GSXR_Device_InputType类型之一, 输出支持的设备输入类型
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则deviceInputTypeFlag为GSXR_Touch或者是GSXR_Joystick类型之一，即*deviceInputTypeFlag = GSXR_Touch 或者*deviceInputTypeFlag =GSXR_Joystick
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则deviceInputTypeFlag为GSXR_Touch或者是GSXR_Joystick类型之一,即*deviceInputTypeFlag = GSXR_Touch 或者*deviceInputTypeFlag =GSXR_Joystick
 *            如果设备类型是GSXR_DeviceType_GamePad，则deviceInputTypeFlag是按GSXR_Gamepad类型之一,即*deviceInputTypeFlag = GSXR_Gamepad
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则deviceInputTypeFlag是按GSXR_Hmd类型,即*deviceInputTypeFlag = GSXR_Hmd
 *            对于其他设备类型暂未开放输入类型，则deviceInputTypeFlag为GSXR_NO_Supported_InputType
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSupportedDeviceInputType(
    GSXR_Runtime        runtime,
    GSXR_DeviceType     deviceType,
    GSXR_Flags64*       deviceInputTypeFlag);

/**
 *  说明：
 *      获取设备连线状态
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *      [out] connected 指向 GSXR_Bool32 数值之指针, 数值输出 GSXR_TRUE 表示已连线, GSXR_FALSE 表示未连线
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
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_IsDeviceConnected(
    GSXR_Runtime        runtime,
    GSXR_DeviceType     deviceType,
    GSXR_Bool32*        connected);

/**
 *  说明：
 *      获取显示设备标识号
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *      [out] displayId 指向 GSXR_DisplayId 标识号之指针, 若设备具显示屏能力则输出显示标识号, 否则输出 GSXR_NULL_DISPLAY_ID
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetDisplay(
    GSXR_Runtime        runtime,
    GSXR_DeviceType     deviceType,
    GSXR_DisplayId*     displayId);

/**
 *  说明：
 *      获取设备输入部件所具备的输入类型配置
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *            目前开放了GSXR_DeviceType_Controller_Left、GSXR_DeviceType_Controller_Right、GSXR_DeviceType_GamePad、GSXR_DeviceType_HMD_VR等设备具备输入类型（GSXR_Device_InputType）  
 *       [in] inputType 指定输入类型,根据GSXR_GetSupportedDeviceInputType获取，并且要求一致
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则inputType为GSXR_Touch或者是GSXR_Joystick类型
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则inputType为GSXR_Touch或者是GSXR_Joystick类型  
 *            如果设备类型是GSXR_DeviceType_GamePad，则inputType是按GSXR_Gamepad类型
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则inputType是按GSXR_Hmd类型
 *            此参数必须与GSXR_GetSupportedDeviceInputType保持一致
 *       [in] inputId 指定输入部件Id
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则inputId是按GSXR_Touch或者是GSXR_Joystick类型下InputId
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则inputId是按GSXR_Touch或者是GSXR_Joystick类型下InputId 
 *            如果设备类型是GSXR_DeviceType_GamePad，则inputId是按GSXR_Gamepad类型下InputId
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则inputId是按GSXR_Hmd类型下InputId
 *            上层根据inputType，进行传递inputId，并转成int类型
 *      [out] configuration 指向 GSXR_Flags64 数值之指针, 数值为 GSXR_InputId_InputType 之位掩码, 输出此输入部件所具备的输入类型配置
 *      案例：(表示摇杆类型下的trigger按键具备按键、触摸、轴值类型)
 *          deviceType：GSXR_DeviceType_Controller_Left
 *          inputType：GSXR_Joystick  
 *          inputId: GSXR_Joystick_InputId_Trigger
 *          *configuration = GSXR_InputId_InputType_Click|GSXR_InputId_InputType_Touch|GSXR_InputId_InputType_Analog_1D
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetInputIdConfiguration(
    GSXR_Runtime                runtime,
    GSXR_DeviceType             deviceType,
    GSXR_Device_InputType       inputType,
    int                         inputId,
    GSXR_Flags64*               configuration);

/**
 *  说明：
 *      获取设备输入部件之点击状态
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *       [in] inputType 指定输入类型,根据GSXR_GetSupportedDeviceInputType获取，并且一致
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则inputType为GSXR_Touch或者是GSXR_Joystick类型
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则inputType为GSXR_Touch或者是GSXR_Joystick类型  
 *            如果设备类型是GSXR_DeviceType_GamePad，则inputType是按GSXR_Gamepad类型
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则inputType是按GSXR_Hmd类型
 *            此参数必须与GSXR_GetSupportedDeviceInputType保持一致
 *      [out] clicks 指向GSXR_Flags64数值之指针，数值按输入类型下的输入ID为位掩码储存InputId的点击状态信息，
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则clicks是按inputType类型下InputId为位掩码来存储点击状态信息
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则clicks是按inputType类型下InputId为位掩码来存储点击状态信息
 *            如果设备类型是GSXR_DeviceType_GamePad，则clicks是按inputType类型下InputId为位掩码来存储按键信息
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则clicks是按inputType类型下InputId为位掩码来存储按键信息
 *            如果设备类型是其他设备，暂时未开放GSXR_Device_InputType定义，暂不支持
 *      案例：(X\Y按键按下)
 *          deviceType：GSXR_DeviceType_Controller_Left
 *          inputType：GSXR_Joystick
 *          *clicks = GSXRMaskFromId(GSXR_Joystick_InputId_X)+GSXRMaskFromId(GSXR_Gamepad_InputId_Y)
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetInputClickStates(
    GSXR_Runtime                runtime,
    GSXR_DeviceType             deviceType, 
    GSXR_Device_InputType       inputType,
    GSXR_Flags64*               clicks);

/**
 *  说明：
 *      获取设备输入部件之触摸状态
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *       [in] inputType 指定输入类型,根据GSXR_GetSupportedDeviceInputType获取，并且一致
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则inputType为GSXR_Touch或者是GSXR_Joystick类型
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则inputType为GSXR_Touch或者是GSXR_Joystick类型  
 *            如果设备类型是GSXR_DeviceType_GamePad，则inputType是按GSXR_Gamepad类型
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则inputType是按GSXR_Hmd类型
 *            此参数必须与GSXR_GetSupportedDeviceInputType保持一致
 *       [in] inputId 指定输入部件
 *      [out] touchs 指向 GSXR_Flags64 数值的指针, 数值按输入类型下的输入ID为位掩码储存InputId的触摸状态信息，
 *           如果设备类型是GSXR_DeviceType_Controller_Left，则touchs是按InputType类型下的InputId为位掩码来存储触摸状态信息
 *           如果设备类型是GSXR_DeviceType_Controller_Right，则touchs是按InputType类型下的InputId为位掩码来存储触摸状态信息
 *           如果设备类型是GSXR_DeviceType_Gamepad，则touchs是按InputType类型下的InputId为位掩码来存储触摸状态信息
 *           如果设备类型是GSXR_DeviceType_HMD_VR，则touchs是按inputType类型下InputId为位掩码来存储按键信息
 *           如果设备类型是其他的设备，暂时未开发GSXR_Device_InputType定义，暂不支持
 *      案例：(X\Y按键触摸)
 *          deviceType：GSXR_DeviceType_Controller_Left
 *          inputType：GSXR_Joystick
 *          *touchs = GSXRMaskFromId(GSXR_Joystick_InputId_X)+GSXRMaskFromId(GSXR_Gamepad_InputId_Y)
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetInputTouchStates(
    GSXR_Runtime                runtime,
    GSXR_DeviceType             deviceType,
    GSXR_Device_InputType       inputType,
    GSXR_Flags64*               touchs);

/**
 *  说明：
 *      获取设备输入部件之类比数据
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *       [in] inputType 指定输入类型,根据GSXR_GetSupportedDeviceInputType获取，并且一致
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则inputType为GSXR_Touch或者是GSXR_Joystick类型
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则inputType为GSXR_Touch或者是GSXR_Joystick类型  
 *            如果设备类型是GSXR_DeviceType_GamePad，则inputType是按GSXR_Gamepad类型
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则inputType是按GSXR_Hmd类型
 *            此参数必须与GSXR_GetSupportedDeviceInputType保持一致
 *       [in] inputId 指定输入部件
 *            如果设备类型是GSXR_DeviceType_Controller_Left，则inputId是按GSXR_Touch或者是GSXR_Joystick类型下InputId
 *            如果设备类型是GSXR_DeviceType_Controller_Right，则inputId是按GSXR_Touch或者是GSXR_Joystick类型下InputId 
 *            如果设备类型是GSXR_DeviceType_GamePad，则inputId是按GSXR_Gamepad类型下InputId
 *            如果设备类型是GSXR_DeviceType_HMD_VR，则inputId是按GSXR_Hmd类型下InputId
 *            上层根据inputType，进行传递inputId，并转成int类型
 *      [out] analog 指向二维矢量 GSXR_Vector2f 结构体之指针, 数据范围见备注说明
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      2. analog 数据范围
 *          a. 若输入部件之输入类型为 GSXR_InputId_InputType_Analog_1D, analog 数据范围为 0<=x<=1, y=0
 *          b. 若输入部件之输入类型为 GSXR_InputId_InputType_Analog_2D, analog 数据范围为 -1<=x<=1, -1<=y<=1
 *  所属：
 *      GSXR_API_设备输入类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetInputAnalogState(
    GSXR_Runtime                runtime,
    GSXR_DeviceType             deviceType,
    GSXR_Device_InputType       inputType,
    int                         inputId,
    GSXR_Vector2f*              analog);

// ------------------------------------
// GSXR_API_设备输出类
// ------------------------------------

// 振动反馈结构体
typedef struct GSXR_HapticVibration {
    void*           next;       // 预留指针
    GSXR_Duration   duration;   // 振动时长, 单位为 nanoseconds 
    uint32_t        frequency;  // 振动次数
    float           intensity;  // 振动强度, 数据介于 0 - 1
} GSXR_HapticVibration;

/**
 *  说明：
 *      触发设备振动反馈, 可自行设置时长,次数,强度, 亦可交由系统触发一个适合当前设备强度适中的短振动
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *       [in] manual 指定手动或自动调控, GSXR_TRUE 表示 XR 应用自行调控振动反馈, GSXR_FALSE 表示交由系统触发一个适合当前设备强度适中的短振动
 *       [in] haptics 指向 GSXR_HapticVibration 结构体之指针, manual 为 GSXR_FALSE 时可输入 nullptr
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备输出类
 */
extern GSXR_EXPORT GSXR_Result GSXR_TriggerVibration(
    GSXR_Runtime                    runtime,
    GSXR_DeviceType                 deviceType,
    GSXR_Bool32                     manual,
    const GSXR_HapticVibration*     haptics);

// ------------------------------------
// GSXR_API_设备空间类
// ------------------------------------

// 枚举空间自由度跟踪能力
typedef enum GSXR_DegreeOfFreedom {
    GSXR_DegreeOfFreedom_None   = 0,    // 无空间跟踪能力
    GSXR_DegreeOfFreedom_3DOF   = 1,    // 只具备旋转跟踪能力
    GSXR_DegreeOfFreedom_6DOF   = 2,    // 兼备旋转及位移跟踪能力
} GSXR_DegreeOfFreedom;

// 枚举空间原点类型
typedef enum GSXR_SpaceOrigin {
    GSXR_SpaceOrigin_OnHead     = 1,    // 原点始于头戴设备本身 (通常用于”没有地板平面”的内容)
    GSXR_SpaceOrigin_OnFloor    = 2,    // 原点考虑身高, 校准至地板上(通常用于”有地板平面”的内容, 需搭配设备的空间校准程序)
} GSXR_SpaceOrigin;


/**
 *  说明：
 *      获取设备的空间自由度跟踪能力
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *      [out] dof 指向 GSXR_DegreeOfFreedom 数值的指针, 输出空间自由度跟踪能力
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetDegreeOfFreedom(
    GSXR_Runtime            runtime,
    GSXR_DeviceType         deviceType,
    GSXR_DegreeOfFreedom*   dof);

/**
 *  说明：
 *      设置 XR 应用之空间原点及偏移量
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] space 指定空间原点类型
 *       [in] offset 指定空间原点偏移量, 若 XR 应用无需自设偏移量可输入 nullptr
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
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetSpaceOrigin(
    GSXR_Runtime        runtime,
    GSXR_SpaceOrigin    space,
    const GSXR_Posef*   offset = nullptr);


/**
 *  说明：
 *      重置 XR 应用之位姿或yaw朝向
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] Yaw 指定是否只重置应用Yaw轴朝向或者应用位姿
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
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ResetOrientationAndPosition(
        GSXR_Runtime        runtime,
        float               Yaw = 0.f);

/**
 *  说明：
 *      获取设备于指定时间之位姿及速度数据
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *       [in] specifiedTime 指定时间
 *      [out] poseState 指向 GSXR_PoseState 结构体之指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetPoseState(
    GSXR_Runtime        runtime,
    GSXR_DeviceType     deviceType,
    GSXR_Time           specifiedTime,
    GSXR_PoseState*     poseState);


// 枚举边界防护墙可见模式
typedef enum GSXR_SpaceBoundsVisible {
    GSXR_SpaceBoundsVisible_System          = 0,    // 交由系统决定边界防护墙显示模式
    GSXR_SpaceBoundsVisible_ForceOn         = 1,    // 强制显示边界防护墙
} GSXR_SpaceBoundsVisible;

// 枚举边界形状
typedef enum GSXR_SpaceBoundsShape {
    GSXR_SpaceBoundsShape_None      = 0,    // 边界未设置
    GSXR_SpaceBoundsShape_Rectangle = 1,    // 矩形边界
    GSXR_SpaceBoundsShape_Circle    = 2,    // 圆形边界
    GSXR_SpaceBoundsShape_Irregular = 3,    // 不规则形边界
} GSXR_SpaceBoundsShape;

// 空间边界配置结构体
typedef struct GSXR_SpaceBoundsConfiguration {
    void*                   next;           // 预留指针
    GSXR_Bool32             isValid;        // GSXR_TRUE 表示边界有效, GSXR_FALSE 表示边界无效
    GSXR_Bool32             isEnabled;      // GSXR_TRUE 表示边界系统开启, GSXR_FALSE 表示边界系统关闭
    GSXR_SpaceBoundsVisible visible;        // 边界防护墙可见模式
    GSXR_SpaceBoundsShape   shape;          // 边界形状
    uint32_t                pointCount;     // 边界形状若为不规则形, pointCount 为点集合数目; 否则为 0
} GSXR_SpaceBoundsConfiguration;

/**
 *  说明：
 *      获取空间边界配置资讯
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] boundsConfiguration 指向 GSXR_SpaceBoundsConfiguration 结构体之指针, 输出边界配置资讯
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSpaceBoundsConfiguration(
    GSXR_Runtime                    runtime,
    GSXR_SpaceBoundsConfiguration*  boundsConfiguration);

/**
 *  说明：
 *      获取矩形边界资讯
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] width 输出矩形边界宽度, 单位为米
 *      [out] length 输出矩形边界长度, 单位为米
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSpaceBoundsRectangle(
    GSXR_Runtime                    runtime,
    float*                          width,
    float*                          length);

/**
 *  说明：
 *      获取圆形边界资讯
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] diameter 输出圆形边界直径, 单位为米
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSpaceBoundsCircle(
    GSXR_Runtime                    runtime,
    float*                          diameter);

/**
 *  说明：
 *      获取不规则形边界资讯
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] pointCount 边界点集合数组个数, 需相等于 GSXR_SpaceBoundsConfiguration 的 pointCount 数
 *      [out] pointArray 指向 GSXR_Posef 结构体之数组, 输出边界点集合位姿
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSpaceBoundsIrregular(
    GSXR_Runtime                    runtime,
    uint32_t                        pointCount,
    GSXR_Posef*                     pointArray);

/**
 *  说明：
 *      设置边界防护墙可见模式
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] visible 指定边界防护墙可见模式
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
 *      GSXR_API_设备空间类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetSpaceBoundsVisible(
    GSXR_Runtime                    runtime,
    GSXR_SpaceBoundsVisible         visible);

// ------------------------------------
// GSXR_API_设备信息类
// ------------------------------------

// 设备属性结构体
typedef struct GSXR_DeviceProperties {
    void*           next;                                           // 预留指针
    char            serialNumber[GSXR_COMMON_STRING_MAX_SIZE];      // 设备 SN 串号
    char            firmwareVersion[GSXR_COMMON_STRING_MAX_SIZE];   // 设备 firmware 版本号
    char            vendorName[GSXR_COMMON_STRING_MAX_SIZE];        // 设备厂家名称
    char            algorithmVersion[GSXR_COMMON_STRING_MAX_SIZE];  // 设备算法版本号
    char            algorithmName[GSXR_COMMON_STRING_MAX_SIZE];     // 设备算法名称
} GSXR_DeviceProperties;

/**
 *  说明：
 *      获取设备属性
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *      [out] deviceProperties 指向 GSXR_DeviceProperties 结构体之指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备信息类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetDeviceProperties(
    GSXR_Runtime            runtime,
    GSXR_DeviceType         deviceType,
    GSXR_DeviceProperties*  deviceProperties);

// 设备电池状态结构体
typedef struct GSXR_BatteryStatus {
    void*           next;           // 预留指针
    float           percentange;    // 电池剩余电量百分比
    GSXR_Bool32     charging;       // GSXR_TRUE 表示电池处于充电状态, GSXR_FALSE 表示电池未处于充电状态
} GSXR_BatteryStatus;

/**
 *  说明：
 *      获取设备电池状态
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] deviceType 指定设备类型
 *      [out] batteryStatus 指向 GSXR_BatteryStatus 结构体之指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Device_Disconnected
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_设备信息类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetBatteryStatus(
    GSXR_Runtime            runtime,
    GSXR_DeviceType         deviceType,
    GSXR_BatteryStatus*     batteryStatus);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_DEVICE_H_ */
