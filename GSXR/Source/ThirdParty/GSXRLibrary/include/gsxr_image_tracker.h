#ifndef GSXR_IMAGE_TRACKER_H_
#define GSXR_IMAGE_TRACKER_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举图像识别特性之功能选项
typedef enum GSXR_ImageRecognitionOptions {
    GSXR_ImageRecognitionOptions_Reserved     = 0xFFFFFFFF,
} GSXR_ImageRecognitionOptions;

// ------------------------------------
// GSXR_API_图像识别类
// ------------------------------------

GSXR_DEFINE_ATOM(GSXR_ImageId)    // 图像标识号

// 图像资讯结构体
typedef struct GSXR_ImageInfo {
    void*           next;   // 预留指针
    const char*     uri;    // 图像资源标识符
    int32_t         left;   // 图像识别区域起始点 x 轴位置, 单位为像素
    int32_t         top;    // 图像识别区域起始点 y 轴位置, 单位为像素
    int32_t         width;  // 图像识别区域宽度, 单位为像素
    int32_t         height; // 图像识别区域高度, 单位为像素
} GSXR_ImageInfo;

/**
 *  说明：
 *      加载欲识别的模型图像
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] imageInfo 指向 GSXR_ImageInfo 结构体, 指定模型图像资讯
 *      [out] modelId 若模型图像加载成功, 输出模型图像标识号
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Out_Of_Memory
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_图像识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_LoadModelImage(
    GSXR_XrFeature          feature,
    const GSXR_ImageInfo*   imageInfo,
    GSXR_ImageId*           modelId);

/**
 *  说明：
 *      卸载模型图像
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] modelId GSXR_ImageId 标识号, 由 GSXR_LoadModelImage 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_图像识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_UnloadModelImage(
    GSXR_XrFeature      feature,
    GSXR_ImageId        modelId);

// 图像识别请求资讯结构体
typedef struct GSXR_ImageTrackerAcquireInfo {
    void*                   next;               // 预留指针
    const GSXR_ImageInfo*   targetImageInfo;    // 待检目标图像
    uint32_t                sourceImageCount;   // 欲识别的源图像数目
    const GSXR_ImageId*     sourceImages;       // 欲识别的源图像数组
    float                   minConfidence;      // 设置识别结果的最小识别置信度, 介于 0 - 1
} GSXR_ImageTrackerAcquireInfo;

// 图像标签结构体
typedef struct GSXR_ImageLabel {
    void*           next;           // 预留指针
    GSXR_ImageId    imageId;        // 目标图像标识号
    int32_t         left;           // 标签区域起始点 x 轴位置, 单位为像素
    int32_t         top;            // 标签区域起始点 y 轴位置, 单位为像素
    int32_t         width;          // 标签区域宽度, 单位为像素
    int32_t         height;         // 标签区域高度, 单位为像素
} GSXR_ImageLabel;

// 图像匹配数据
typedef struct GSXR_ImageMatchData {
    void*               next;           // 预留指针
    GSXR_ImageLabel     target;         // 识别出的目标图像标签
    GSXR_ImageId        source;         // 识别出的对应源图像标识号
    float               confidence;     // 识别置信度, 介于 0 - 1
} GSXR_ImageMatchData;

// 图像识别结果数据结构体
typedef struct GSXR_ImageTrackerData {
    void*                   next;           // 预留指针
    uint32_t                matchCount;     // 在目标图像上识别出的图像个数
    GSXR_ImageMatchData*    matches;        // 识别出的图像匹配资讯
} GSXR_ImageTrackerData;

/**
 *  说明：
 *      获得图像识别结果数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] acquireInfo 指向 GSXR_ImageTrackerAcquireInfo 结构体, 指定图像识别请求资讯
 *      [out] imageTracker 指向 GSXR_ImageTrackerData 指针
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Out_Of_Memory
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      imageTracker 指针指向的 GSXR_ImageTrackerData 指针指向图像识别特性的 GSXR_ImageTrackerData 结构,
 *      应用程序用毕图像识别结果数据必须调用 GSXR_ReleaseImageTrackerData 释放内存
 *  所属：
 *      GSXR_API_图像识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_AcquireImageTrackerData(
    GSXR_XrFeature                          feature,
    const GSXR_ImageTrackerAcquireInfo*     acquireInfo,
    GSXR_ImageTrackerData**                 imageTracker);

/**
 *  说明：
 *      释放图像识别结果数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] imageTracker GSXR_ImageTrackerData 指针, 由 GSXR_AcquireImageTrackerData 获取
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Call_Flow_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      无
 *  所属：
 *      GSXR_API_图像识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ReleaseImageTrackerData(
    GSXR_XrFeature              feature,
    GSXR_ImageTrackerData*      imageTracker);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_IMAGE_TRACKER_H_ */
