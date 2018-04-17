//
//  QHVCEditComand.h
//  QHVCEditKit
//
//  Created by liyue-g on 2017/9/11.
//  Copyright © 2017年 liyue-g. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#pragma mark - 日志相关
#pragma mark -

typedef NS_ENUM(NSUInteger, QHVCEditLogLevel)
{
    QHVCEditLogLevel_Error,  //仅包含错误
    QHVCEditLogLevel_Warn,   //错误+警告
    QHVCEditLogLevel_Info,   //错误+警告+状态信息
    QHVCEditLogLevel_Debug,  //错误+警告+状态信息+调试信息
};

@interface QHVCEditLog : NSObject

/**
 获取版本号
 */
+ (NSString *)getVersion;

/**
 设置日志级别

 @param level 日志级别
 */
+ (void)setLogLevel:(QHVCEditLogLevel)level;

@end

#pragma mark - 获取文件信息

typedef NS_ENUM(NSUInteger, QHVCEditVideoCodec)
{
    QHVCEditVideoCodec_H264,
    QHVCEditVideoCodec_HEVC,
};

typedef NS_ENUM(NSUInteger, QHVCEditAudioCodec)
{
    QHVCEditAudioCodec_AAC,
};

@interface QHVCEditFileInfo : NSObject
@property (nonatomic, assign) BOOL isPicture;                //是否是图片
@property (nonatomic, assign) NSInteger width;               //宽度
@property (nonatomic, assign) NSInteger height;              //高度
@property (nonatomic, assign) NSInteger durationMs;          //文件总时长(单位：毫秒)
@property (nonatomic, assign) NSInteger videoBitrate;        //视频码率
@property (nonatomic, assign) NSInteger fps;                 //视频帧率
@property (nonatomic, assign) NSInteger audioBitrate;        //音频码率
@property (nonatomic, assign) NSInteger audioChannels;       //音频声道数
@property (nonatomic, assign) NSInteger audioSamplerate;     //音频采样率

@end

@interface QHVCEditGetFileInfo : NSObject

+ (QHVCEditFileInfo *)getFileInfo:(NSString *)filePath; //获取文件信息

@end

#pragma mark - 输出相关
#pragma mark - 

typedef NS_ENUM(NSUInteger, QHVCEditOutputError)
{
    QHVCEditOutputError_NoError      = 1, //无错误
    QHVCEditOutputError_ParamError   = 2, //参数错误
};

typedef NS_ENUM(NSUInteger, QHVCEditOutputRenderMode)
{
    QHVCEditOutputRenderMode_AspectFit,    //视频内容完全填充，可能会有黑边
    QHVCEditOutputRenderMode_AspectFill,   //视频内容铺满画布，视频内容可能会被裁剪
    QHVCEditOutputRenderMode_ScaleToFill,  //视频内容铺满画布，视频内容可能会被拉伸
};

typedef NS_ENUM(NSUInteger, QHVCEditOutputBackgroundMode)
{
    QHVCEditOutputBackgroundMode_Color,  //纯色背景
    QHVCEditOutputBackgroundMode_Blur,   //毛玻璃背景
};

@interface QHVCEditOutputParams : NSObject

@property (nonatomic, assign) QHVCEditOutputRenderMode renderMode; //视频帧渲染模式，默认黑边填充QHVCEditOutputRenderMode_AspectFit
@property (nonatomic, assign) QHVCEditOutputBackgroundMode backgroundMode; //视频背景画布样式，默认黑色背景
@property (nonatomic, retain) NSString* backgroundInfo; //背景信息 (背景样式为 QHVCEditOutputBackgroudMode_Color时，背景信息为16进制ARGB值)
@property (nonatomic, assign) CGSize size;

@end

#pragma mark - 指令相关
#pragma mark -

typedef NS_ENUM(NSUInteger, QHVCEditCommandError)
{
    QHVCEditCommandError_NoError       = 1, //无错误
    QHVCEditCommandError_ParamError    = 2, //参数错误
    QHVCEditCommandError_FactoryError  = 3, //指令工厂错误
    QHVCEditCommandError_SendError     = 4, //指令发送错误
};

@interface QHVCEditSegmentInfo : NSObject
@property (nonatomic, strong) NSString* filePath;         //物理文件路径
@property (nonatomic, assign) NSInteger fileDuration;     //物理文件时长
@property (nonatomic, assign) NSInteger segmentStartTime; //片段开始时间点，相对物理文件
@property (nonatomic, assign) NSInteger segmentEndTime;   //片段结束时间点，相对物理文件

@end

@interface QHVCEditCommandFactory : NSObject

- (instancetype)initCommandFactory; //初始化指令工厂
- (QHVCEditCommandError)freeCommandFactory; //释放指令工厂

//获取文件片段列表信息block (片段信息数组，片段总时长)
typedef void(^QHVCEditSegmentInfoBlock)(NSArray<QHVCEditSegmentInfo *>* segments, NSInteger totalDurationMs);
- (QHVCEditCommandError)getSegmentInfo:(QHVCEditSegmentInfoBlock)complete; //获取文件片段列表信息

@property (nonatomic, retain) QHVCEditOutputParams* defaultOutputParams; //默认输出样式
@property (nonatomic, readonly, assign) NSInteger factoryHandle;

@end

@interface QHVCEditCommand : NSObject

- (instancetype)initCommand:(QHVCEditCommandFactory *)commandFactory; //创建指令
- (QHVCEditCommandError)addCommand; //添加指令

@property (nonatomic, readonly, assign) NSInteger commandId;  //指令ID

@end

@interface QHVCEditEditableCommand : QHVCEditCommand

- (QHVCEditCommandError)editCommand;   //修改已发送指令
- (QHVCEditCommandError)deleteCommand; //删除已发送指令

@end

#pragma mark - 添加视频文件

@interface QHVCEditCommandAddVideoFile : QHVCEditCommand
@property (nonatomic, strong) NSString* filePath;  //视频存储路径
@property (nonatomic, assign) NSInteger fileIndex; //插入序号

@end

#pragma mark - 添加文件片段

@interface QHVCEditCommandAddFileSegment : QHVCEditCommand
@property (nonatomic, strong) NSString* filePath;              //视频存储路径
@property (nonatomic, assign) NSInteger fileIndex;             //插入序号
@property (nonatomic, assign) NSTimeInterval durationMs;       //持续时长(单位：毫秒)，类型为视频文件时，赋值为<=0的任意数值，类型为图片文件时，赋值为实际持续时长
@property (nonatomic, assign) NSTimeInterval startTimestampMs; //起始时间点，相对物理文件时间(单位：毫秒),文件为图片时此值无效
@property (nonatomic, assign) NSTimeInterval endTimestampMs;   //结束时间点，相对物理文件时间(单位：毫秒),文件为图片时此值无效
@property (nonatomic, assign) BOOL mute;                       //是否舍弃音频
@property (nonatomic, assign) int volume;                      //音量0~100
@property (nonatomic, assign) float speed;                     //0.25~4

@end

#pragma mark - 添加图片文件

@interface QHVCEditCommandAddImageFile : QHVCEditCommand
@property (nonatomic, strong) NSString* filePath;        //图片文件路径
@property (nonatomic, assign) NSTimeInterval durationMs; //图片重复时长(单位：毫秒)
@property (nonatomic, assign) NSInteger fileIndex;       //插入序号

@end

#pragma mark - 分割文件

@interface QHVCEditCommandCutFile : QHVCEditCommand
@property (nonatomic, assign) NSInteger fileIndex;           //文件序列号
@property (nonatomic, assign) NSTimeInterval cutTimestampMs; //剪切时间点，相对所有文件所在时间轴（单位：毫秒）

@end

#pragma mark - 移动文件

@interface QHVCEditCommandMoveFile : QHVCEditCommand
@property (nonatomic, assign) NSInteger fileIndex; //文件序列号
@property (nonatomic, assign) NSInteger moveStep;  //移动位数，负数左移，正数右移

@end

#pragma mark - 删除文件

@interface QHVCEditCommandDeleteFile : QHVCEditCommand
@property (nonatomic, assign) NSInteger fileIndex; //文件序列号

@end

#pragma mark - 文件合成

@interface QHVCEditCommandMakeFile : QHVCEditCommand
@property (nonatomic, strong) NSString* filePath; //文件导出路径

@end

#pragma mark - 背景音乐

@interface QHVCEditCommandBackgroundMusic : QHVCEditEditableCommand
@property (nonatomic, strong) NSString* filePath;               //音频文件路径
@property (nonatomic, assign) NSTimeInterval insertTimeStampMs; //插入时间点, 相对所有文件所在时间轴(单位：毫秒)
@property (nonatomic, assign) NSTimeInterval endTimeStampMs;    //插入结束时间点, 相对所有文件所在时间轴（单位：毫秒）
@property (nonatomic, assign) NSInteger volume;                 //音频音量(0~100)
@property (nonatomic, assign) BOOL loop;                        //是否循环播放

@end

#pragma mark - 音频素材

@interface QHVCEditCommandAudio : QHVCEditEditableCommand
@property (nonatomic, strong) NSString* filePath;                 //音频文件全路径
@property (nonatomic, assign) NSTimeInterval startTime;           //音频文件开始时间，相对素材(单位：毫秒)
@property (nonatomic, assign) NSTimeInterval endTime;             //音频文件结束时间，相对素材（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval insertStartTime;     //音频文件混音相对于所有文件所在的时间轴的开始时间（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval insertEndTime;       //音频文件混音相对于所有文件所在的时间轴的结束时间 (单位：毫秒)
@property (nonatomic, assign) int volume;                         //音频音量(0~100)
@property (nonatomic, assign) BOOL loop;                          //是否循环播放

@end

#pragma mark -  调节文件音量

@interface QHVCEditCommandAlterVolume : QHVCEditCommand
@property (nonatomic, assign) NSInteger fileIndex;           //文件序列号
@property (nonatomic, assign) int volume;                    //音量0~100

@end

#pragma mark -  文件变速

@interface QHVCEditCommandAlterSpeed : QHVCEditCommand
@property (nonatomic, assign) NSInteger fileIndex;           //文件序列号
@property (nonatomic, assign) float speed;                   //速度0.25~4

@end

#pragma mark - 叠加片段

@interface QHVCEditCommandOverlaySegment : QHVCEditEditableCommand
@property (nonatomic, strong) NSString* filePath;                 //物理文件路径
@property (nonatomic, assign) NSTimeInterval durationMs;          //素材持续时长，单位毫秒，当持续时长大于素材片段时长时，按循环处理
@property (nonatomic, assign) NSTimeInterval startTimestampMs;    //素材相对于物理文件的起始时间，单位毫秒，对于图片文件开始时间为0
@property (nonatomic, assign) NSTimeInterval endTimestampMs;      //素材相对于物理文件的结束时间，单位毫秒，对于图片文件结束时间为0
@property (nonatomic, assign) NSTimeInterval insertTimestampMs;   //叠加片段插入时间点，相对所有文件所在时间轴（单位：毫秒）
@property (nonatomic, assign) NSInteger volume;                   //音量值（0-100，默认为100）
@property (nonatomic, assign) CGFloat speed;                      //速率（0.25~4）

@end

#pragma mark - 矩阵操作

@interface QHVCEditCommandMatrixFilter : QHVCEditEditableCommand

//指定特效添加层级。添加给某个叠加片段，需指定叠加文件的commandId; 只添加给主片段列表，=0; 默认为-1，生效于主片段和所有叠加片段
@property (nonatomic, assign) NSInteger overlayCommandId;
@property (nonatomic, assign) CGFloat frameRotateAngle;                 //视频帧相对画布旋转弧度值，例如，90° = π/2，默认不旋转
@property (nonatomic, assign) CGFloat previewRotateAngle;               //视频画布旋转弧度值，例如，90° = π/2，默认不旋转
@property (nonatomic, assign) BOOL flipX;                               //是否左右镜像，默认不镜像
@property (nonatomic, assign) BOOL flipY;                               //是否上下镜像，默认不镜像
@property (nonatomic, assign) CGRect renderRect;                        //绘制点矩阵，相对目标画布，默认对齐画布原点、画布大小
@property (nonatomic, assign) CGRect sourceRect;                        //截取素材矩阵，相对源素材，默认对齐源素材原点、源素材大小
@property (nonatomic, assign) NSInteger renderZOrder;                   //渲染层级，层级越低越靠下，层级越高越靠上，默认0
@property (nonatomic, strong) UIView* preview;                          //渲染窗口，可为空
@property (nonatomic, strong) QHVCEditOutputParams* outputParams;       //输出渲染样式，可为空，默认同CommandFactory defaultOutputParams
@property (nonatomic, assign) NSTimeInterval startTimestampMs;          //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;            //结束时间点，相对所有文件所在时间轴，开始时间和结束不能跨片段（单位：毫秒

@end

#pragma mark - 贴图操作

@interface QHVCEditCommandImageFilter : QHVCEditEditableCommand

//指定特效添加层级。添加给某个叠加片段，需指定叠加文件的commandId; 只添加给主片段列表，=0; 默认为-1，生效于主片段和所有叠加片段
@property (nonatomic, assign) NSInteger overlayCommandId;
@property (nonatomic, strong) UIImage* image;                  //贴图
@property (nonatomic, strong) NSString* imagePath;             //贴图物理路径，路径和贴图都存在时优先读取路径
@property (nonatomic, assign) CGFloat destinationX;            //绘制点x坐标，相对目标画布
@property (nonatomic, assign) CGFloat destinationY;            //绘制点y坐标，相对目标画布
@property (nonatomic, assign) CGFloat destinationWidth;        //绘制点宽度，相对目标画布
@property (nonatomic, assign) CGFloat destinationHeight;       //绘制点高度，相对目标画布
@property (nonatomic, assign) CGFloat destinationRotateAngle;  //绘制旋转弧度值，例如，90° = π/2
@property (nonatomic, assign) NSTimeInterval startTimestampMs; //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;   //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end

#pragma mark - 字幕操作

@interface QHVCEditCommandSubtitle : QHVCEditEditableCommand

//指定特效添加层级。添加给某个叠加片段，需指定叠加文件的commandId; 只添加给主片段列表，=0; 默认为-1，生效于主片段和所有叠加片段
@property (nonatomic, assign) NSInteger overlayCommandId;
@property (nonatomic, strong) UIImage* image;                  //字幕图
@property (nonatomic, strong) NSString* imagePath;             //字幕图物理路径，路径和贴图都存在时优先读取路径
@property (nonatomic, assign) CGFloat destinationX;            //绘制点x坐标，相对目标画布
@property (nonatomic, assign) CGFloat destinationY;            //绘制点y坐标，相对目标画布
@property (nonatomic, assign) CGFloat destinationWidth;        //绘制点宽度，相对目标画布
@property (nonatomic, assign) CGFloat destinationHeight;       //绘制点高度，相对目标画布
@property (nonatomic, assign) CGFloat destinationRotateAngle;  //绘制旋转旋转弧度值，例如，90° = π/2
@property (nonatomic, assign) NSTimeInterval startTimestampMs; //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;   //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end

#pragma mark - 水印操作

@interface QHVCEditCommandWaterMark : QHVCEditEditableCommand

//指定特效添加层级。添加给某个叠加片段，需指定叠加文件的commandId; 只添加给主片段列表，=0; 默认为-1，生效于主片段和所有叠加片段
@property (nonatomic, assign) NSInteger overlayCommandId;
@property (nonatomic, strong) UIImage* image;                  //水印图
@property (nonatomic, strong) NSString* imagePath;             //水印图物理路径，路径和贴图都存在时优先读取路径
@property (nonatomic, assign) CGFloat destinationX;            //绘制点x坐标，相对目标画布
@property (nonatomic, assign) CGFloat destinationY;            //绘制点y坐标，相对目标画布
@property (nonatomic, assign) CGFloat destinationWidth;        //绘制点宽度，相对目标画布
@property (nonatomic, assign) CGFloat destinationHeight;       //绘制点高度，相对目标画布
@property (nonatomic, assign) CGFloat destinationRotateAngle;  //绘制旋转旋转弧度值，例如，90° = π/2
@property (nonatomic, assign) NSTimeInterval startTimestampMs; //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;   //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end

#pragma mark - 滤镜操作

typedef NS_ENUM(NSUInteger, QHVCEditAuxFilterType)
{
    QHVCEditAuxFilterType_Color,        //叠加颜色
    QHVCEditAuxFilterType_CLUT,         //查色图
};

@interface QHVCEditCommandAuxFilter : QHVCEditEditableCommand

//指定特效添加层级。添加给某个叠加片段，需指定叠加文件的commandId; 只添加给主片段列表，=0; 默认为-1，生效于主片段和所有叠加片段
@property (nonatomic, assign) NSInteger overlayCommandId;
@property (nonatomic, assign) QHVCEditAuxFilterType auxFilterType; //滤镜类型
@property (nonatomic, retain) NSString* auxFilterInfo;             //滤镜信息。滤镜类型为color时，为填充背景色 (16进制值, ARGB)；滤镜类型为CLUT时，为查色图路径
@property (nonatomic, assign) CGFloat progress;                    //滤镜程度(0-1)，默认为1
@property (nonatomic, assign) NSTimeInterval startTimestampMs;     //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;       //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end

#pragma mark - 画质操作

typedef NS_ENUM(NSUInteger, QHVCEditQualityType)
{
    QHVCEditQualityType_exposure,       //曝光度
    QHVCEditQualityType_gamma,          //伽马值
    QHVCEditQualityType_saturation,     //饱和度
    QHVCEditQualityType_brightness,     //亮度
    QHVCEditQualityType_contrast,       //对比度
    QHVCEditQualityType_hue,            //色相
};

@interface QHVCEditCommandQualityFilter : QHVCEditEditableCommand

//指定特效添加层级。添加给某个叠加片段，需指定叠加文件的commandId; 只添加给主片段列表，=0; 默认为-1，生效于主片段和所有叠加片段
@property (nonatomic, assign) NSInteger overlayCommandId;
@property (nonatomic, assign) QHVCEditQualityType qualityType; //画质类型
@property (nonatomic, assign) CGFloat qualityValue;            //画质值（-1~1，默认为0）
@property (nonatomic, assign) NSTimeInterval startTimestampMs; //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;   //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end

#pragma mark - 转场操作

typedef NS_ENUM(NSUInteger, QHVCEditTransitionType)
{
    QHVCEditTransitionType_dissolution,     //溶解
    QHVCEditTransitionType_aperture,        //光圈
    QHVCEditTransitionType_swipeToRight,    //向右轻擦
    QHVCEditTransitionType_swipeToLeft,     //向左轻擦
    QHVCEditTransitionType_swipeToTop,      //向上轻擦
    QHVCEditTransitionType_swipeToBottom,   //向下轻擦
    QHVCEditTransitionType_moveToRight,     //向右滑动
    QHVCEditTransitionType_moveToLeft,      //向左滑动
    QHVCEditTransitionType_moveToTop,       //向上滑动
    QHVCEditTransitionType_moveToBottom,    //向下滑动
    QHVCEditTransitionType_fade,            //淡化
};

@interface QHVCEditCommandTransition : QHVCEditEditableCommand
@property (nonatomic, assign) NSInteger overlayCommandId;                   //需指定叠加片段的commandId
@property (nonatomic, assign) QHVCEditTransitionType transitionType;        //转场类型
@property (nonatomic, assign) NSTimeInterval startTimestampMs;              //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;                //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end

#pragma mark - 色键抠图

@interface QHVCEditCommandChromakey : QHVCEditEditableCommand

//指定特效添加层级。添加给某个叠加片段，需指定叠加文件的commandId; 只添加给主片段列表，=0; 默认为-1，生效于主片段和所有叠加片段
@property (nonatomic, assign) NSInteger overlayCommandId;
@property (nonatomic, retain) NSString* color;                              //抠去的颜色, 16进制ARGB值
@property (nonatomic, assign) int threshold;                                //基于抠去的颜色的波动范围，阈值越大抠色范围越大, 0-100
@property (nonatomic, assign) int extend;                                   //扩展（0-10）,在最大、最小色调区间外再扩展一定范围的过度区间，让抠图效果更自然
@property (nonatomic, assign) NSTimeInterval startTimestampMs;              //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;                //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end











