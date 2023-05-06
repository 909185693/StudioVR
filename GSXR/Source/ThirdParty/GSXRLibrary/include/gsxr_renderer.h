#ifndef GSXR_RENDERER_H_
#define GSXR_RENDERER_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_types.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

GSXR_DEFINE_HANDLE(GSXR_Renderer)       // 渲染器句柄
GSXR_DEFINE_HANDLE(GSXR_TextureQueue)   // 纹理队列句柄
GSXR_DEFINE_HANDLE(GSXR_Texture)        // 纹理句柄
GSXR_DEFINE_HANDLE(GSXR_RenderLooper)   // 渲染循环句柄

// 枚举视图类型
typedef enum GSXR_ViewType {
    GSXR_ViewType_None              = 0,    // 无效的视图

    GSXR_ViewType_Primary_Mono      = 1,    // 主屏单视图
    GSXR_ViewType_Primary_Stereo    = 2,    // 主屏双视图
    
    GSXR_ViewType_Secondary_Mono    = 17,   // 副屏单视图
    GSXR_ViewType_Secondary_Stereo  = 18,   // 副屏双视图
} GSXR_ViewType;

// 渲染器状态事件结构体
typedef struct GSXR_RendererStateEventData {
    GSXR_Renderer       renderer;       // 渲染器句柄
    GSXR_ViewType       viewType;       // 视图类型
    GSXR_TextureQueue   textureQueue;   // 纹理队列句柄
    GSXR_RenderLooper   looper;         // 渲染循环句柄
} GSXR_RendererStateEventData;

// ------------------------------------
// GSXR_API_渲染视图类
// ------------------------------------

// 枚举图形API类型
typedef enum GSXR_GraphicsApi {
    GSXR_GraphicsApi_OpenGLES   = 0,    // OpenGL ES
    GSXR_GraphicsApi_OpenGL     = 1,    // OpenGL
} GSXR_GraphicsApi;

// 渲染器初始资讯结构体
typedef struct GSXR_RendererCreateInfo {
    void*               next;               // 预留指针
    GSXR_GraphicsApi    graphicsApi;        // 图形 API 类型
    GSXR_DisplayId      displayId;          // GSXR_DisplayId 标识号, 由 GSXR_GetDisplay 获取
} GSXR_RendererCreateInfo;

/**
 *  说明：
 *      创建渲染器
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] createInfo 指向 GSXR_RendererCreateInfo 结构体, 内含渲染器初始资讯
 *      [out] renderer 指向 GSXR_Renderer 句柄之指针, 渲染器创建成功输出一有效句柄, 创建失败输出 GSXR_NULL_HANDLE
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Out_Of_Memory
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染视图类
 */
extern GSXR_EXPORT GSXR_Result GSXR_CreateRenderer(
    GSXR_Runtime                        runtime,
    const GSXR_RendererCreateInfo*      createInfo, 
    GSXR_Renderer*                      renderer);

/**
 *  说明：
 *      销毁渲染器
 *  参数：
 *       [in] renderer GSXR_Renderer 句柄, 由 GSXR_CreateRenderer 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染视图类
 */
extern GSXR_EXPORT GSXR_Result GSXR_DestroyRenderer(
    GSXR_Renderer       renderer);

// 视图集配置结构体
typedef struct GSXR_ViewSetConfiguration {
    void*           next;                   // 预留指针
    GSXR_ViewType   viewType;               // 视图类型 (同一渲染器下的各视图集配置 viewType 不重复)
    uint32_t        viewCount;              // 视图集之视图数目
    uint32_t        textureFormatCount;     // 纹理队列支持的格式数目
    uint32_t        targetWidth;            // 纹理图像目标宽度
    uint32_t        maxWidth;               // 纹理图像最大宽度
    uint32_t        targetHeight;           // 纹理图像目标高度
    uint32_t        maxHeight;              // 纹理图像最大高度
    uint32_t        targetSampleCount;      // 纹理图像目标采样数
    uint32_t        maxSampleCount;         // 纹理图像最大采样数
    uint32_t        fps;                    // 渲染帧率
    uint32_t        maxLayerCount;          // 渲染合成层最大层数
    uint32_t        ipd;                    // 瞳距
    float           fovHRad;                // 水平FOV
    float           fovVRad;                // 垂直FOV
} GSXR_ViewSetConfiguration;

/**
 *  说明：
 *      获取渲染器的视图集数目
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *      [out] count 视图集数目
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染视图类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetViewSetCount(
    GSXR_Runtime        runtime,
    uint32_t*           count);

/**
 *  说明：
 *      获取渲染器的视图集配置
 *  参数：
 *       [in] runtime GSXR_Runtime 句柄, 由 GSXR_CreateRuntime 获取
 *       [in] count configurations 数组个数, 需相等于 GSXR_GetViewSetCount 的 count 数
 *      [out] configurations 指向 GSXR_ViewSetConfiguration 结构体之数组
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染视图类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetViewSetConfigurations(
    GSXR_Runtime                    runtime,
    uint32_t                        count,
    GSXR_ViewSetConfiguration*      configurations);

// ------------------------------------
// GSXR_API_渲染纹理类
// ------------------------------------

typedef int64_t GSXR_TextureFormat;

// 纹理队列功能选项
typedef enum GSXR_TextureQueueOptions {
    GSXR_TextureQueueOptions_Protected_Content  = 0x00000001,   // 纹理内存受保护
    GSXR_TextureQueueOptions_Color_Attachment   = 0x00010000,   // 纹理附加颜色属性
    GSXR_TextureQueueOptions_Manual_Management  = 0x10000000,   // 应用自建纹理队列
} GSXR_TextureQueueOptions;

/**
 *  说明：
 *      获取纹理队列功能选项, 包含建议的及所有的支持选项
 *  参数：
 *       [in] renderer GSXR_Renderer 句柄, 由 GSXR_CreateRenderer 获取
 *      [out] recommendedOptions GSXR_TextureQueueOptions 之位掩码, 输出系統建議的纹理队列功能选项
 *      [out] supportedOptions GSXR_TextureQueueOptions 之位掩码, 输出可支持的所有纹理队列功能选项
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染纹理类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetTextureQueueOptions(
    GSXR_Renderer                   renderer,
    GSXR_Flags64*                   recommendedOptions,
    GSXR_Flags64*                   supportedOptions);

/**
 *  说明：
 *      获取纹理队列支持的纹理格式
 *  参数：
 *       [in] renderer GSXR_Renderer 句柄, 由 GSXR_CreateRenderer 获取
 *       [in] formatCount formats 数组个数, 需相等于 GSXR_ViewSetConfiguration 的 textureFormatCount 数
 *      [out] formats 指向 GSXR_TextureFormat 之数组
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染纹理类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetSupportedTextureFormats(
    GSXR_Renderer           renderer,
    uint32_t                formatCount,
    GSXR_TextureFormat*     formats);

// 纹理队列初始资讯结构体
typedef struct GSXR_TextureQueueCreateInfo {
    void*                   next;               // 预留指针
    GSXR_Flags64            options;            // 设置启用的纹理功能选项
    GSXR_TextureFormat      format;             // 纹理图像格式, 数值为各图形 API 自定义
    uint32_t                width;              // 纹理图像宽度
    uint32_t                height;             // 纹理图像高度
    uint32_t                sampleCount;        // 纹理图像采样数
    uint32_t                faceCount;          // 纹理图像张数
    uint32_t                mipLevelCount;      // 纹理图像 mipmap 层级数
    uint32_t                arrayLayerCount;    // 纹理图像数组阶层数
} GSXR_TextureQueueCreateInfo;

/**
 *  说明：
 *      创建纹理队列
 *  参数：
 *       [in] renderer GSXR_Renderer 句柄, 由 GSXR_CreateRenderer 获取
 *       [in] createInfo  指向 GSXR_TextureQueueCreateInfo 结构体
 *      [out] textureQueue 纹理队列创建成功输出一有效句柄, 创建失败输出 GSXR_NULL_HANDLE
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染纹理类
 */
extern GSXR_EXPORT GSXR_Result GSXR_CreateTextureQueue(
    GSXR_Renderer                           renderer, 
    const GSXR_TextureQueueCreateInfo*      createInfo, 
    GSXR_TextureQueue*                      textureQueue);

/**
 *  说明：
 *      销毁纹理队列
 *  参数：
 *       [in] textureQueue GSXR_TextureQueue 句柄, 由 GSXR_CreateTextureQueue 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染纹理类
 */
extern GSXR_EXPORT GSXR_Result GSXR_DestroyTextureQueue(
    GSXR_TextureQueue   textureQueue);

/**
 *  说明：
 *      获取纹理队列中的纹理数目
 *  参数：
 *       [in] textureQueue GSXR_TextureQueue 句柄, 由 GSXR_CreateTextureQueue 获取
 *      [out] count 纹理数目
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染纹理类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetTextureCount(
    GSXR_TextureQueue       textureQueue,
    uint32_t*               count);


// 纹理元素结构体
typedef struct GSXR_TextureElement {
    void*           next;       // 预留指针
    uint32_t        index;      // 纹理元素于纹理队列中的索引, 从 0 起算
    GSXR_Texture    texture;    // 纹理句柄
} GSXR_TextureElement;
    
/**
 *  说明：
 *      等待纹理队列获得可用纹理
 *  参数：
 *       [in] textureQueue GSXR_TextureQueue 句柄, 由 GSXR_CreateTextureQueue 获取
 *       [in] timeout 持续等待的超时时限值, 单位为 naroseconds
 *      [out] textureElement 指向 GSXR_TextureElement 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Result_Timeout_Expired
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染纹理类
 */
extern GSXR_EXPORT GSXR_Result GSXR_WaitAndAcquireAvailableTexture(
    GSXR_TextureQueue       textureQueue,
    GSXR_Duration           timeout,
    GSXR_TextureElement*    textureElement);

/**
 *  说明：
 *      将纹理释放回纹理队列
 *  参数：
 *       [in] textureQueue GSXR_TextureQueue 句柄, 由 GSXR_CreateTextureQueue 获取
 *       [in] textureElement 指定释放的纹理元素, 由 GSXR_WaitAndAcquireAvailableTexture 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染纹理类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ReleaseTexture(
    GSXR_TextureQueue               textureQueue,
    const GSXR_TextureElement*      textureElement);

// ------------------------------------
// GSXR_API_渲染循环类
// ------------------------------------
typedef enum GSXR_ColorSpace
{
    GSXR_ColorSpace_sRGB =1,
    GSXR_ColorSpace_Linear=2
}GSXR_ColorSpace;

// 渲染循环开始资讯结构体
typedef struct GSXR_RenderLooperStartInfo {
    void*                   next;           // 预留指针
    GSXR_ViewType           viewType;       // 指定视图类型, 由 GSXR_GetViewSetConfigurations 获取
    const void*             graphicsBinding;    // 连接图形API特化资讯 (在移动设备系统, graphicsBinding 指向 GSXR_AndrosOpenGLESBinding 结构体)
    void*                   m_androsAppNativeWindow; // 应用的 ANativeWindow
    GSXR_ColorSpace         m_colorSpace;
} GSXR_RenderLooperStartInfo;
    
/**
 *  说明：
 *      开始渲染循环
 *  参数：
 *       [in] renderer GSXR_Renderer 句柄, 由 GSXR_CreateRenderer 获取
 *       [in] startInfo 指向 GSXR_RenderLooperStartInfo 结构体, 内含渲染循环开始资讯
 *      [out] looper 渲染循环创建成功输出一有效句柄, 创建失败输出 GSXR_NULL_HANDLE
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Out_Of_Memory
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      渲染器状态需处于 GSXR_EventType_RendererState_Available 方能开始渲染循环
 *  所属：
 *      GSXR_API_渲染视图类
 */
extern GSXR_EXPORT GSXR_Result GSXR_StartRenderLooper(
    GSXR_Renderer                           renderer,
    const GSXR_RenderLooperStartInfo*       startInfo, 
    GSXR_RenderLooper*                      looper);
    
/**
 *  说明：
 *      停止渲染循环
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染视图类
 */
extern GSXR_EXPORT GSXR_Result GSXR_StopRenderLooper(
    GSXR_RenderLooper       looper);

/**
 *  说明：
 *      设置 XR 应用渲染循环线程 id, Runtime 赋予此线程较高的线程优先权
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *       [in] threadId 应用渲染循环线程 id
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SetRenderLooperThread(
    GSXR_RenderLooper       looper,
    uint32_t                threadId);

/**
 *  说明：
 *      等待调节帧渲染时机，并输出渲染幀的预测显示时间， XR 应用需依据此时间获取对应姿态进行渲染
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *      [out] predictedDisplayTime 输出应用渲染此幀后的显示时间
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_WaitFrame(
    GSXR_RenderLooper       looper,
    GSXR_Time*              predictedDisplayTime);

// 视场角结构体
typedef struct GSXR_Fovf {
    float    angleLeft;     // 左视场角, 单位为弧度
    float    angleRight;    // 右视场角, 单位为弧度
    float    angleUp;       // 上视场角, 单位为弧度
    float    angleDown;     // 下视场角, 单位为弧度
} GSXR_Fovf;

// 视图姿态结构体
typedef struct GSXR_ViewPoseState {
    GSXR_Posef      pose;   // 位姿结构体
    GSXR_Fovf       fov;    // 视场角结构体
} GSXR_ViewPoseState;

/**
 *  说明：
 *      获取视图于指定时间之姿态
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *       [in] specifiedTime 指定时间
 *       [in] viewCount viewPoseState 数组个数, 需相等于 GSXR_ViewSetConfiguration 的 viewCount 数
 *      [out] viewPoseState 指向 GSXR_ViewPoseState 结构体之数组
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetViewPoseState(
    GSXR_RenderLooper       looper,
    GSXR_Time               specifiedTime,
    uint32_t                viewCount,
    GSXR_ViewPoseState*     viewPoseState);

// 枚举合成层类型
typedef enum GSXR_CompositionLayerType {
    GSXR_CompositionLayerType_Projection    = 0,    // 三维场景投影层
    GSXR_CompositionLayerType_Quad          = 1,    // 二维四方平面层
} GSXR_CompositionLayerType;

// 枚举合成层功能选项
typedef enum GSXR_CompositionLayerOptions {
    GSXR_CompositionLayerOptions_Distortion_Correction  = 0x00000001,   // 畸变校正
    GSXR_CompositionLayerOptions_Alpha_Blending         = 0x00000002,   // Alpha 混合
} GSXR_CompositionLayerOptions;

// 渲染合成层标头结构体
typedef struct GSXR_CompositionLayerHeader {
    void*                           next;           // 预留指针
    GSXR_CompositionLayerType       layerType;      // 合成层类型
    GSXR_Flags64                    layerOptions;   // 设置启用的合成层功能选项
} GSXR_CompositionLayerHeader;

// 纹理子图像结构体
typedef struct GSXR_TexSubImage {
    void*               next;                   // 预留指针
    GSXR_TextureQueue   textureQueue;           // 纹理队列句柄
    GSXR_TextureElement textureElement;         // 纹理元素
    int32_t             xOffset;                // 纹理子图像 x 偏移量
    int32_t             yOffset;                // 纹理子图像 y 偏移量
    int32_t             width;                  // 纹理子图像宽度
    int32_t             height;                 // 纹理子图像高度
    uint32_t            arrayLayerIndex;        // 纹理子图像数组索引
} GSXR_TexSubImage;

typedef enum GSXR_Eye
{
    GSXR_Eye_Left = 1,                         //左眼
    GSXR_Eye_Right = 2,                        //右眼
    GSXR_Eye_Both = 3                          //双眼
}GSXR_Eye;


// 合成层视图资讯结构体
typedef struct GSXR_CompositionLayerView {
    void*                   next;               // 预留指针
    GSXR_ViewPoseState      viewPoseState;      // 视图姿态
    GSXR_TexSubImage        texSubImage;        // 纹理子图像资讯
    GSXR_Eye                eyeMask;            //
} GSXR_CompositionLayerView;

// 三维场景投影层结构体
typedef struct GSXR_CompositionLayerProjection {
    void*                                       next;           // 预留指针
    GSXR_CompositionLayerType                   layerType;      // 合成层类型, GSXR_CompositionLayerType_Projection
    GSXR_Flags64                                layerOptions;   // 设置启用的合成层功能选项
    uint32_t                                    viewCount;      // 视图数目
    const GSXR_CompositionLayerView*            views;          // 投影层视图资讯数组
} GSXR_CompositionLayerProjection;

// 二维四方平面层结构体
typedef struct GSXR_CompositionLayerQuad {
    void*                                       next;           // 预留指针
    GSXR_CompositionLayerType                   layerType;      // 合成层类型, GSXR_CompositionLayerType_Quad
    GSXR_Flags64                                layerOptions;   // 设置启用的合成层功能选项
    uint32_t                                    viewCount;      // 视图数目
    const GSXR_CompositionLayerView*            views;          // 视图资讯数组
    GSXR_Bool32                                 isOriginOnView; // 平面层位姿是否以视图为原点
    GSXR_Posef                                  quadPose;       // 平面层位姿
    float                                       width;          // 平面层宽度, 单位为米
    float                                       height;         // 平面层高度, 单位为米
} GSXR_CompositionLayerQuad;

// 渲染帧提交资讯结构体
typedef struct GSXR_FrameSubmitInfo {
    void*                                       next;           // 预留指针
    GSXR_Time                                   displayTime;    // 渲染帧的显示时间
    uint32_t                                    layerCount;     // 合成层数
    const GSXR_CompositionLayerHeader* const*   layers;         // 合成层指针的数组
} GSXR_FrameSubmitInfo;

/**
 *  说明：
 *      提交应用的渲染帧资讯给 Runtime 渲染管线进行后续处理
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *       [in] submitInfo  指向 GSXR_FrameSubmitInfo 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_SubmitFrame(
    GSXR_RenderLooper               looper,
    const GSXR_FrameSubmitInfo*     submitInfo);



// 枚举渲染功能类型
typedef enum GSXR_RenderFunctionType {
    GSXR_RenderFunctionType_Foveated            = 0x00000001,   // 注视点渲染
} GSXR_RenderFunctionType;

/**
 *  说明：
 *      获取渲染功能
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *      [out] recommendedFunctions GSXR_RenderFunctionType 之位掩码, 输出系統建議的渲染功能
 *      [out] supportedFunctions GSXR_RenderFunctionType 之位掩码, 输出可支持的所有渲染功能
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetRenderFunctions(
    GSXR_RenderLooper               looper,
    GSXR_Flags64*                   recommendedFunctions,
    GSXR_Flags64*                   supportedFunctions);

// 渲染功能标头结构体
typedef struct GSXR_RenderFunctionHeader {
    void*                           next;               // 预留指针
    GSXR_RenderFunctionType         functionType;       // 渲染功能类型
    GSXR_Bool32                     enable;             // GSXR_TRUE 表示启用此渲染功能(通常用于 GSXR_PreRenderFrame), GSXR_FALSE 表示停用此渲染功能(通常用于 GSXR_PostRenderFrame)
} GSXR_RenderFunctionHeader;

// 枚举注视点周围视觉品质等级
typedef enum GSXR_PeripheralQuality {
    GSXR_PeripheralQuality_System    = 0,               // 系统预设视觉品质等级
    GSXR_PeripheralQuality_High      = 1,               // 高视觉品质等级 (注视点周围轻度模糊)
    GSXR_PeripheralQuality_Medium    = 2,               // 中视觉品质等级 (注视点周围中度模糊)
    GSXR_PeripheralQuality_Low       = 3,               // 低视觉品质等级 (注视点周围重度模糊)
} GSXR_PeripheralQuality;

// 注视点渲染参数结构体
typedef struct GSXR_FoveatedParameters {
    void*                       next;                   // 预留指针
    GSXR_Vector2f               focal;                  // 注视点位置坐标, 数据范围为 -1 <= x <= 1, -1 <= y <= 1
    GSXR_Fovf                   foveaFov;               // 注视点视场角
    GSXR_PeripheralQuality      peripheralQuality;      // 注视点周围视觉品质等级
} GSXR_FoveatedParameters;

// 注视点视图资讯结构体
typedef struct GSXR_FoveatedView {
    void*                           next;               // 预留指针
    GSXR_TexSubImage                texSubImage;        // 纹理子图像资讯
    const GSXR_FoveatedParameters*  foveatedParams;     // 注视点渲染参数
} GSXR_FoveatedView;

// 注视点渲染功能结构体
typedef struct GSXR_RenderFunctionFoveated {
    void*                           next;               // 预留指针
    GSXR_RenderFunctionType         functionType;       // 渲染功能类型, GSXR_RenderFunctionType_Foveated
    GSXR_Bool32                     enable;             // GSXR_TRUE 表示启用注视点渲染, GSXR_FALSE 表示停用注视点渲染
    uint32_t                        viewCount;          // 视图数目
    const GSXR_FoveatedView*        foveatedViews;      // 注视点视图资讯
} GSXR_RenderFunctionFoveated;

// 渲染帧功能资讯结构体
typedef struct GSXR_FrameRenderInfo {
    void*                                       next;               // 预留指针
    uint32_t                                    functionCount;      // 渲染功能数目
    const GSXR_RenderFunctionHeader* const*     functions;          // 渲染功能指针的数组
} GSXR_FrameRenderInfo;

/**
 *  说明：
 *      于应用进行渲染前, 预设置渲染功能参数予应用的渲染纹理, 通常用于渲染循环中启用渲染功能
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *       [in] preRenderInfo  指向 GSXR_FrameRenderInfo 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      必须在应用进行渲染前调用
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_PreRenderFrame(
    GSXR_RenderLooper               looper,
    const GSXR_FrameRenderInfo*     preRenderInfo);

/**
 *  说明：
 *      于应用进行渲染后, 后设置渲染功能参数予应用的渲染纹理, 通常用于渲染循环中停用渲染功能
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *       [in] postRenderInfo  指向 GSXR_FrameRenderInfo 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      必须在应用进行渲染后调用
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_PostRenderFrame(
    GSXR_RenderLooper               looper,
    const GSXR_FrameRenderInfo*     postRenderInfo);


/**
 *  说明：
 *      获取现存注视点参数
 *  参数：
 *       [in] looper GSXR_RenderLooper 句柄, 由 GSXR_StartRenderLooper 获取
 *      [out] enabled 输出注视点渲染是否启用中, GSXR_TRUE 为启用, GSXR_FALSE 为停用
 *       [in] viewCount foveatedParams 数组个数, 需相等于 GSXR_ViewSetConfiguration 的 viewCount 数
 *      [out] foveatedParams 指向 GSXR_FoveatedParameters 结构体之数组
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Function_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Renderer_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_渲染循环类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetFoveatedParameters(
    GSXR_RenderLooper           looper,
    GSXR_Bool32*                enabled,
    uint32_t                    viewCount,
    GSXR_FoveatedParameters*    foveatedParams);


#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_RENDERER_H_ */
