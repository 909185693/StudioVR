#ifndef GSXR_FACE_TRACKER_H_
#define GSXR_FACE_TRACKER_H_

/**
 *  Declare GSXR Copyright here.
 */

#include "gsxr_xrfeature.h"
#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

// 枚举脸部识别特性之功能选项
typedef enum GSXR_FaceRecognitionOptions {
    GSXR_FaceRecognitionOptions_Reserved     = 0xFFFFFFFF,
} GSXR_FaceRecognitionOptions;

// ------------------------------------
// GSXR_API_脸部识别类
// ------------------------------------

GSXR_DEFINE_ATOM(GSXR_FaceId)    // 脸部标识号

// 脸部图像资讯结构体
typedef struct GSXR_FaceImageInfo {
    void*           next;   // 预留指针
    const char*     uri;    // 脸部图像资源标识符
    int32_t         left;   // 脸部图像识别区域起始点 x 轴位置, 单位为像素
    int32_t         top;    // 脸部图像识别区域起始点 y 轴位置, 单位为像素
    int32_t         width;  // 脸部图像识别区域宽度, 单位为像素
    int32_t         height; // 脸部图像识别区域高度, 单位为像素
} GSXR_FaceImageInfo;

// 脸部识别请求资讯结构体
typedef struct GSXR_FaceTrackerAcquireInfo {
    void*                       next;               // 预留指针
    const GSXR_FaceImageInfo*   targetImageInfo;    // 待检脸部目标图像
    float                       minConfidence;      // 设置识别结果的最小识别置信度, 介于 0 - 1
} GSXR_FaceTrackerAcquireInfo;

// 脸部标签结构体
typedef struct GSXR_FaceLabel {
    void*           next;           // 预留指针
    GSXR_FaceId     facelId;        // 脸部标识号
    int32_t         left;           // 标签区域起始点 x 轴位置, 单位为像素
    int32_t         top;            // 标签区域起始点 y 轴位置, 单位为像素
    int32_t         width;          // 标签区域宽度, 单位为像素
    int32_t         height;         // 标签区域高度, 单位为像素
    float           confidence;     // 脸部识别置信度, 介于 0 - 1
} GSXR_FaceLabel;

// 脸部识别结果数据结构体
typedef struct GSXR_FaceTrackerData {
    void*               next;           // 预留指针
    GSXR_Time           timestamp;      // 时间戳
    uint32_t            labelCount;     // 在图像上识别出的脸部标签个数
    GSXR_FaceLabel*     labels;         // 脸部标签资讯
} GSXR_FaceTrackerData;

/**
 *  说明：
 *      获得脸部识别结果数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] acquireInfo 指向 GSXR_FaceTrackerAcquireInfo 结构体, 指定脸部识别请求资讯
 *      [out] faceTracker 指向 GSXR_FaceTrackerData 指针
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
 *      faceTracker 指针指向的 GSXR_FaceTrackerData 指针指向脸部识别特性的 GSXR_FaceTrackerData 结构,
 *      应用程序用毕脸部识别结果数据必须调用 GSXR_ReleaseFaceTrackerData 释放内存
 *  所属：
 *      GSXR_API_脸部识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_AcquireFaceTrackerData(
    GSXR_XrFeature                          feature,
    const GSXR_FaceTrackerAcquireInfo*      acquireInfo,
    GSXR_FaceTrackerData**                  faceTracker);

/**
 *  说明：
 *      释放脸部识别结果数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] faceTracker GSXR_FaceTrackerData 指针, 由 GSXR_AcquireFaceTrackerData 获取
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
 *      GSXR_API_脸部识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ReleaseFaceTrackerData(
    GSXR_XrFeature              feature,
    GSXR_FaceTrackerData*       faceTracker);



// 脸部比对请求资讯结构体
typedef struct GSXR_FaceMatchTrackerAcquireInfo {
    void*                       next;               // 预留指针
    const GSXR_FaceImageInfo*   targetImageInfo;    // 待检目标图像
    uint32_t                    sourceFaceCount;    // 欲比对的源脸部数目
    const GSXR_FaceLabel*       sourceFaces;        // 欲比对的源脸部数组
    float                       minSimilarity;      // 设置识别结果的最小识别相似度, 介于 0 - 1
} GSXR_FaceMatchTrackerAcquireInfo;

// 脸部比对数据结构体
typedef struct GSXR_FaceMatchData {
    void*               next;           // 预留指针
    GSXR_FaceLabel      target;         // 识别出的目标脸部标签
    GSXR_FaceLabel      source;         // 识别出的对应源脸部标签
    float               similarity;     // 脸部相似度, 介于 0 - 1
} GSXR_FaceMatchData;

// 脸部比对结果数据结构体
typedef struct GSXR_FaceMatchTrackerData {
    void*                   next;           // 预留指针
    GSXR_Time               timestamp;      // 时间戳
    uint32_t                matchCount;     // 在目标图像上识别出的脸部个数
    GSXR_FaceMatchData*     matches;        // 识别的脸部资讯
} GSXR_FaceMatchTrackerData;

/**
 *  说明：
 *      获得脸部比对结果数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] acquireInfo 指向 GSXR_FaceMatchTrackerAcquireInfo 结构体, 指定脸部比对请求资讯
 *      [out] faceMatchTracker 指向 GSXR_FaceMatchTrackerData 指针
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
 *      faceMatchTracker 指针指向的 GSXR_FaceMatchTrackerData 指针指向脸部识别特性的 GSXR_FaceMatchTrackerData 结构,
 *      应用程序用毕脸部比对结果数据必须调用 GSXR_ReleaseFaceMatchTrackerData 释放内存
 *  所属：
 *      GSXR_API_脸部识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_AcquireFaceMatchTrackerData(
    GSXR_XrFeature                              feature,
    const GSXR_FaceMatchTrackerAcquireInfo*     acquireInfo,
    GSXR_FaceMatchTrackerData**                 faceMatchTracker);

/**
 *  说明：
 *      释放脸部比对结果数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *      [out] faceMatchTracker GSXR_FaceMatchTrackerData 指针, 由 GSXR_AcquireFaceMatchTrackerData 获取
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
 *      GSXR_API_脸部识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_ReleaseFaceMatchTrackerData(
    GSXR_XrFeature                  feature,
    GSXR_FaceMatchTrackerData*      faceMatchTracker);


// 脸部特征点坐标结构体
typedef struct GSXR_FacialPoint {
    int32_t     x;
    int32_t     y;
} GSXR_FacialPoint;

// 脸部部位特征点数据结构体
typedef struct GSXR_FacialPortionData {
    void*               next;               // 预留指针
    uint32_t            featureCount;       // 部位特征点数目
    GSXR_Bool32*        isFeatureValid;     // 部位特征点有效性数组
    GSXR_FacialPoint*   features;           // 部位特征点坐标数组
} GSXR_FacialPortionData;


#define GSXR_FACIAL_MAIN_FEATURE_COUNT 5

// 枚举脸部主特征点
typedef enum GSXR_FacialMainFeature {
    GSXR_FacialMainFeature_Eye_Left     = 0,
    GSXR_FacialMainFeature_Eye_Right    = 1,
    GSXR_FacialMainFeature_Nose         = 2,
    GSXR_FacialMainFeature_Mouth_Left   = 3,
    GSXR_FacialMainFeature_Mouth_Right  = 4,
} GSXR_FacialMainFeature;

// 脸部主特征点数据结构体
typedef struct GSXR_FacialMainFeatureData {
    void*                       next;       // 预留指针
    GSXR_FacialPortionData      main;       // 脸部主特征点数据, featureCount 为 GSXR_FACIAL_MAIN_FEATURE_COUNT
} GSXR_FacialMainFeatureData;


#define GSXR_EYEBROW_FEATURE_COUNT 5

// 枚举眉毛特征点
typedef enum GSXR_EyebrowFeature {
    GSXR_EyebrowFeature_Middle          = 0,
    GSXR_EyebrowFeature_Left            = 1,
    GSXR_EyebrowFeature_Right           = 2,
    GSXR_EyebrowFeature_Middle_Left     = 3,
    GSXR_EyebrowFeature_Middle_Right    = 4,
} GSXR_EyebrowFeature;


#define GSXR_EYE_FEATURE_COUNT 11

// 枚举眼睛特征点
typedef enum GSXR_EyeFeature {
    GSXR_EyeFeature_Pupil               = 0,
    GSXR_EyeFeature_Socket_Left         = 1,
    GSXR_EyeFeature_Socket_Right        = 2,
    GSXR_EyeFeature_Socket_Bottom       = 3,
    GSXR_EyeFeature_Socket_Top          = 4,
    GSXR_EyeFeature_Socket_Left_Bottom  = 5,
    GSXR_EyeFeature_Socket_Right_Bottom = 6,
    GSXR_EyeFeature_Socket_Left_Top     = 7,
    GSXR_EyeFeature_Socket_Right_Top    = 8,
    GSXR_EyeFeature_Iris_Left           = 9,
    GSXR_EyeFeature_Iris_Right          = 10,
} GSXR_EyeFeature;


#define GSXR_NOSE_FEATURE_COUNT 9

// 枚举鼻子特征点
typedef enum GSXR_NoseFeature {
    GSXR_NoseFeature_Tip                = 0,
    GSXR_NoseFeature_Bottom             = 1,
    GSXR_NoseFeature_Bridge             = 2,
    GSXR_NoseFeature_Wing_Left          = 3,
    GSXR_NoseFeature_Wing_Right         = 4,
    GSXR_NoseFeature_Wing_Left_Outer    = 5,
    GSXR_NoseFeature_Wing_Right_Outer   = 6,
    GSXR_NoseFeature_Wing_Left_Lower    = 7,
    GSXR_NoseFeature_Wing_Right_Lower   = 8,
} GSXR_NoseFeature;


#define GSXR_MOUTH_FEATURE_COUNT 14

// 枚举嘴巴特征点
typedef enum GSXR_MouthFeature {
    GSXR_MouthFeature_Left                      = 0,
    GSXR_MouthFeature_Right                     = 1,
    GSXR_MouthFeature_Lip_Bottom                = 2,
    GSXR_MouthFeature_Lip_Bottom_Inner          = 3,
    GSXR_MouthFeature_Lip_Top                   = 4,
    GSXR_MouthFeature_Lip_Top_Inner             = 5,
    GSXR_MouthFeature_Lip_Left_Bottom           = 6,
    GSXR_MouthFeature_Lip_Left_Bottom_Inner     = 7,
    GSXR_MouthFeature_Lip_Right_Bottom          = 8,
    GSXR_MouthFeature_Lip_Right_Bottom_Inner    = 9,
    GSXR_MouthFeature_Lip_Left_Top              = 10,
    GSXR_MouthFeature_Lip_Left_Top_Inner        = 11,
    GSXR_MouthFeature_Lip_Right_Top             = 12,
    GSXR_MouthFeature_Lip_Right_Top_Inner       = 13,
} GSXR_MouthFeature;


#define GSXR_CONTOUR_FEATURE_COUNT 9

// 枚举脸廓特征点
typedef enum GSXR_ContourFeature {
    GSXR_ContourFeature_Jawline_Left_Upper      = 0,
    GSXR_ContourFeature_Jawline_Right_Upper     = 1,
    GSXR_ContourFeature_Jawline_Left_Middle     = 2,
    GSXR_ContourFeature_Jawline_Right_Middle    = 3,
    GSXR_ContourFeature_Jawline_Left_Lower      = 4,
    GSXR_ContourFeature_Jawline_Right_Lower     = 5,
    GSXR_ContourFeature_Chin_Left               = 6,
    GSXR_ContourFeature_Chin_Right              = 7,
    GSXR_ContourFeature_Chin_Bottom             = 8,
} GSXR_ContourFeature;

// 脸部详细特征点数据结构体
typedef struct GSXR_FacialDetailFeatureData {
    void*                       next;           // 预留指针
    GSXR_FacialPortionData      leftEyebrow;    // 左眉毛特征点数据, featureCount 为 GSXR_EYEBROW_FEATURE_COUNT
    GSXR_FacialPortionData      rightEyebrow;   // 右眉毛特征点数据, featureCount 为 GSXR_EYEBROW_FEATURE_COUNT
    GSXR_FacialPortionData      leftEye;        // 左眼特征点数据, featureCount 为 GSXR_EYE_FEATURE_COUNT
    GSXR_FacialPortionData      rightEye;       // 右眼特征点数据, featureCount 为 GSXR_EYE_FEATURE_COUNT
    GSXR_FacialPortionData      nose;           // 鼻子特征点数据, featureCount 为 GSXR_NOSE_FEATURE_COUNT
    GSXR_FacialPortionData      mouth;          // 嘴巴特征点数据, featureCount 为 GSXR_MOUTH_FEATURE_COUNT
    GSXR_FacialPortionData      contour;        // 脸廓特征点数据, featureCount 为 GSXR_CONTOUR_FEATURE_COUNT
} GSXR_FacialDetailFeatureData;


/**
 *  说明：
 *      获取脸部主特征点数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] facelId 指定脸部标识号
 *      [out] data 指向 GSXR_FacialMainFeatureData 结构体
 *  返回值：
 *      GSXR_Result_Success
 *      GSXR_Error_Operation_Failed
 *      GSXR_Error_Feature_Unsupported
 *      GSXR_Error_Handle_Invalid
 *      GSXR_Error_Argument_Invalid
 *      GSXR_Error_Runtime_Unavailable
 *      GSXR_Error_Feature_Unavailable
 *  备注：
 *      GSXR_FacialPortionData 中的特征点数组内存由应用程序根据特征点数目配置
 *  所属：
 *      GSXR_API_脸部识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetFacialMainFeatureData(
    GSXR_XrFeature                  feature,
    GSXR_FaceId                     facelId,
    GSXR_FacialMainFeatureData*     data);

/**
 *  说明：
 *      获取脸部详细特征点数据
 *  参数：
 *       [in] feature GSXR_XrFeature 句柄, 由 GSXR_StartXrFeature 获取
 *       [in] facelId 指定脸部标识号
 *      [out] data 指向 GSXR_FacialDetailFeatureData 结构体
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
 *      GSXR_FacialPortionData 中的特征点数组内存由应用程序根据特征点数目配置
 *  所属：
 *      GSXR_API_脸部识别类
 */
extern GSXR_EXPORT GSXR_Result GSXR_GetFacialDetailFeatureData(
    GSXR_XrFeature                  feature,
    GSXR_FaceId                     facelId,
    GSXR_FacialDetailFeatureData*   data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "close_code.h"

#endif /* GSXR_FACE_TRACKER_H_ */
