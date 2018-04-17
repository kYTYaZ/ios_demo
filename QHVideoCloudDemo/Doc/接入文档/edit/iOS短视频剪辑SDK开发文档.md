# iOS短视频剪辑SDK开发文档

### 介绍：
360直播云以SDK形式提供短视频剪辑服务，可以帮助开发者快速集成短视频剪辑能力。SDK包含framework、demo及开发文档。

### 功能说明：
短视频剪辑SDK支持文件导入、编辑、添加特效、合成导出等功能。
具体功能列表如下：

1.文件导入：

|功能列表							|
|-------------------------|
|支持导入图片、gif、视频     |
|支持导入文件片段				|

2.缩略图

|功能列表							|
|-------------------------|
|支持以任意粒度获取文件缩略图  |
|支持取消获取缩略图操作			|
	
3.文件编辑：

|功能列表							|
|-------------------------|
|排序								|
|剪切								|
|删除								|
|设置渲染模式（黑边填充、裁剪填充、变形填充）|
|设置背景样式（rgba色值、毛玻璃效果）|
|添加背景音乐						|
|添加音乐素材						|
|添加画中画素材					|
|调节文件音量						|
|音视频分离						|
|文件变速							|

4.添加特效

|功能列表							|
|-------------------------|
|添加静态字幕						|
|添加静态贴纸						|
|添加滤镜							|
|添加水印、二维码				|
|画面旋转							|
|画面水平、垂直镜像				|
|调整画质（曝光度、伽马值、饱和度、亮度、对比度、色相）|
|添加转场							|
|色键抠图							|
	
5.实时预览

|功能列表							|
|-------------------------|
|播放								|
|暂停								|
|跳转到某个时间点				|
|刷新播放器						|
|重置播放器						|
|获取当前播放时间				|
|获取当前视频帧					|
	
6.合成

|功能列表							|
|-------------------------|
|开始合成							|
|支持取消正在进行的合成操作		|
	
### 系统范围：
| 系统特性 | 支持范围       |
| ------- | :----------: |
| 系统版本 | iOS 9+        |
| 系统架构 | armv7、armv64 |

### 系统框图：

### SDK集成方法介绍：

#### 下载工程，demo对接

demo下载链接：https://github.com/360livecloud/iOS_demo.git
sdk下载链接：https://live.360.cn/index/sdkdownload

#### 函数及配置说明
#### 添加依赖库
TARGETS -> General -> Embed Binaries

QHVCEditKit.framework

#### 引入头文件
```
#import <QHVCEditKit/QHVCEditKit.h>          //头文件列表

或
#import <QHVCEditKit/QHVCEditPlayer.h>	     //实时预览
#import <QHVCEditKit/QHVCEditCommand.h>      //指令操作
#import <QHVCEditKit/Thumbnail.h>			 //缩略图
#import <QHVCEditKit/Maker.h>				 //合成操作
```

#### 初始化指令工厂
所有操作都基于指令完成，使用时需先初始化指令工厂，再根据不同的动作创建不同的指令。

```
//初始化指令工厂
commandFactory = [[QHVCEditCommandFactory alloc] initCommandFactory];

//配置输出样式
outputParams = [[QHVCEditOutput alloc] init];
[outputParams setOutputSize:CGSizeMake(360, 640)];
[outputParams setOutputRenderMode:QHVCEditOutputRenderMode_AspectFit];
[commandFactory setOutput:outputParams];

```
#### 指令操作
指令分为两大类：不可编辑指令和可编辑指令。不可编辑指令只有initCommand（初始化）、addCommand（添加指令）方法，可编辑指令有initCommand、addCommand、editCommand、deleteCommand方法，分别对应指令的初始化、增、改、删操作。

- 不可编辑指令：

```Objective-C
//添加视频文件
QHVCEditCommandAddVideoFile* command = [[QHVCEditCommandAddVideoFile alloc] initCommand:self.commandFactory];
[command setFilePath:item.filePath];
[command setFileIndex:index];
[command addCommand];
```

```Objective-C
//添加图片文件
QHVCEditCommandAddImageFile* command = [[QHVCEditCommandAddImageFile alloc] initCommand:self.commandFactory];
[command setDurationMs:3*1000];
[command setFilePath:item.filePath];
[command setFileIndex:index];
[command addCommand];
```

```Objective-C
//添加文件片段
QHVCEditCommandAddVideoFileSegment* command = [[QHVCEditCommandAddVideoFileSegment alloc] initCommand:self.commandFactory];
[command setFilePath:item.filePath];
if (!item.isVideo)
{
    [command setDurationMs:3*1000];
}
else
{
    [command setStartTimestampMs:2000];
    [command setEndTimestampMs:3000];
}
[command addCommand];
```

```Objective-C
//文件剪切
 QHVCEditCommandCutFile* command = [[QHVCEditCommandCutFile alloc] initCommand:self.commandFactory];
[command setFileIndex:self.clipFileIndex];
[command setCutTimestampMs:self.clipTimestampMs];
[command addCommand];
```

```Objective-C
//文件删除
QHVCEditCommandDeleteFile* command = [[QHVCEditCommandDeleteFile alloc] initCommand:self.commandFactory];
[command setFileIndex:index];
[command addCommand];
```

```Objective-C
//文件移动
QHVCEditCommandMoveFile* command = [[QHVCEditCommandMoveFile alloc] initCommand:self.commandFactory];
[command setFileIndex:index];
[command setMoveStep:step];
[command addCommand];
```

```Objective-C
//配置文件合成参数
self.fileSavePath = [NSString stringWithFormat:@"%@%@", NSTemporaryDirectory(), @"edit.mp4"];
QHVCEditCommandMakeFile* command = [[QHVCEditCommandMakeFile alloc] initCommand:self.commandFactory];
[command setFilePath:self.fileSavePath];
[command addCommand];
```
- 可编辑指令：

```Objective-C
//背景音乐指令
//NSString* filePath = [[NSBundle mainBundle] pathForResource:@"sunshine" ofType:@"mp3"];
self.bgMusicCommand = [[QHVCEditCommandBackgroundMusic alloc] initCommand:self.commandFactory];
[self.bgMusicCommand setFilePath:filePath];
[self.bgMusicCommand setInsertTimeStampMs:self.curStartTimestamp];
[self.bgMusicCommand setEndTimeStampMs:self.totalDurationMs];
[self.bgMusicCommand setVolum:50];
[self.bgMusicCommand setLoop:YES];

//添加
[self.bgMusicCommand addCommand];

//修改
//[self.bgMusicCommand editCommand];

//删除
//[self.bgMusicCommand deleteCommand];
```

```Objective-C
//音乐素材
QHVCEditCommandAudio *a = [[QHVCEditCommandAudio alloc]initCommand:self.commandFactory];
NSString* filePath = [[NSBundle mainBundle] pathForResource:item.audiofile ofType:@"mp3"];
a.filePath = filePath;
a.startTime = item.startTimeMs;
a.endTime = item.audioDuration;
a.insertStartTime = item.insertStartTimeMs;
a.insertEndTime = item.insertEndTimeMs;
a.volume = item.volume;
a.loop = (a.endTime - a.startTime) > (a.insertEndTime - a.insertStartTime)?NO:YES;

//添加
[a addCommand];

//修改
//[a editCommand];

//删除
//[a deleteCommand];
```

```Objective-C
//叠加文件
QHVCEditCommandOverlaySegment* cmd = [[QHVCEditCommandOverlaySegment alloc] initCommand:self.commandFactory];
cmd.filePath = file.filePath;
cmd.durationMs = file.endMs - file.startMs;
cmd.startTimestampMs = file.asset.mediaType == PHAssetMediaTypeImage ? 0 : file.startMs;
cmd.endTimestampMs = file.asset.mediaType == PHAssetMediaTypeImage ? 0 : file.endMs;
cmd.insertTimestampMs = 0;

//添加
[cmd addCommand];

//修改
//[cmd editCommand];

//删除
//[cmd deleteCommand];
```

```Objective-C
//调节文件音量
 QHVCEditCommandAlterVolume *cmd = [[QHVCEditCommandAlterVolume alloc]initCommand:self.commandFactory];
 cmd.fileIndex = i;
 cmd.volume = item.volume;

 //添加
 [cmd addCommand];

 //修改
 //[a editCommand];

 //删除
 //[a deleteCommand];
```

```Objective-C
//矩阵指令(旋转、水平翻转)
command = [[QHVCEditCommandMatrixFilter alloc] initCommand:self.commandFactory];
[command setDestinationRotateAngle:0.25];
[command setFlipX:YES];
[command setStartTimestampMs:self.curStartTimestamp];
[command setEndTimestampMs:self.curEndTimestamp];

//添加
[command addCommand];

//修改
//[command editCommand];

//删除
//[command deleteCommand];

```

```Objective-C
//文件变速
QHVCEditCommandAlterSpeed *command = [[QHVCEditCommandAlterSpeed alloc] initCommand:self.commandFactory];
[command setFileIndex:i];
[command setSpeed:speed];

//添加
[command addCommand];

//修改
//[command editCommand];

//删除
//[command deleteCommand];
```

```Objective-C
//字幕指令
command = [[QHVCEditCommandSubtitle alloc] initCommand:self.commandFactory];
CGFloat x = CGRectGetMinX(self.curFrameRectInView);
CGFloat scale = self.curFrameImage.size.height/CGRectGetHeight(self.curFrameRectInView);
command.image = image;
command.destinationX = (CGRectGetMinX(subtitle.frame) - x)*scale;
command.destinationY = CGRectGetMinY(subtitle.frame)*scale;
command.destinationWidth = CGRectGetWidth(subtitle.frame)*scale;
command.destinationHeight = CGRectGetHeight(subtitle.frame)*scale;
command.destinationRotateAngle = 0;
command.startTimestampMs = self.curStartTimestampMs;
command.endTimestampMs = self.curEndTimestampMs;

//添加
[command addCommand];

//修改
//[command editCommand];

//删除
//[command deleteCommand];                  
```

贴图指令、水印指令使用方式同字幕指令。

```Objective-C
 //新增滤镜
command = [[QHVCEditCommandAuxFilter alloc] initCommand:self.commandFactory];
command.auxFilterType = self.curAuxFilterType;
command.startTimestampMs = self.curStartTimestampMs;
command.endTimestampMs = self.curEndTimestampMs;

//添加
[command addCommand];

//修改
//[command editCommand];

//删除
//[command deleteCommand];
```

```Objective-C
//画质指令
QHVCEditCommandQualityFilter *q = [[QHVCEditCommandQualityFilter alloc] initCommand:self.commandFactory];
q.qualityType = type;
q.qualityValue = value;
q.startTimestampMs = start;
q.endTimestampMs = q.startTimestampMs +cmd.endTimestampMs;

//添加
[q addCommand];

//修改
//[q editCommand];

//删除
//[q deleteCommand];
```

```Objective-C
//转场指令
QHVCEditCommandTransition *transferCommand = [[QHVCEditCommandTransition alloc]initCommand:self.commandFactory];
transferCommand.overlayCommandId = overlaySegment.commandId;
transferCommand.transitionType = (QHVCEditTransitionType)item.transferType;
transferCommand.startTimestampMs = MIN(originalSegment.segmentStartTime - kTransferDuration, 0) ;
transferCommand.endTimestampMs = originalSegment.segmentStartTime;

//添加
[transferCommand addCommand];

//修改
//[transferCommand editCommand];

//删除
//[transferCommand deleteCommand];
```

```Objective-C
//色键抠图
QHVCEditCommandChromakey* cmd = [[QHVCEditCommandChromakey alloc] initCommand:self.commandFactory];
cmd.overlayCommandId = overlayCommandId;
cmd.minHueAngle = minHue;
cmd.maxHueAngle = maxHue;
cmd.extend = extend;
cmd.startTimestampMs = startTimestampMs;
cmd.endTimestampMs = endTimestampMs;

//添加
[cmd addCommand];

//修改
//[cmd editCommand];

//删除
//[cmd deleteCommand];
```

#### 实时预览

- 初始化

```Objective-C
self.player = [[QHVCEditPlayer alloc] initPlayerWithCommandFactory:self.commandFactory];
[self.player setPreviewBackgroudColor:@"FF4B4B4B"];
[self.player setPlayerDelegate:self];
```

- 设置预览画面

```Objective-C
[self.player setPlayerPreview:self.playerPreview];
```
- 播放

```Objective-C
[self.player playerPlay];
```

- 暂停

```Objective-C
[self.player playerStop];
```

- 释放

```Objective-C
[self.player free];
```

- 跳转到某个时间点

```Objective-C
[self.player playerSeekToTime:time complete:^(NSTimeInterval currentTime){}];
```

- 刷新播放器

```Objective-C
[self.player refresh];
```

- 重置播放器

```Objective-C
[self.player reset];
```

- 获取当前播放时间戳

```Objective-C
NSTimeInterval timestamp = [self.player getCurrentTimestamp];
```

- 获取当前视频帧

```Objective-C
UIImage* image = [self.player getCurrentFrame];
```

- 协议部分

```Objective-C
/**
 播放器错误回调

 @param error 错误类型
 @param info 详细错误信息
 */
- (void)onPlayerError:(QHVCEditPlayerError)error detailInfo:(QHVCEditPlayerErrorInfo)info;
```

```Objective-C
/**
 播放完成回调
 */
- (void)onPlayerPlayComplete;
```

#### 缩略图

- 初始化

```Objective-C
self.thumbnailFactory = [[QHVCEditThumbnail alloc] initThumbnailFactory];
```

- 获取缩略图

```Objective-C
 [self.thumbnailFactory getVideoThumbnailFromFile:fileItem.filePath
                                               width:200
                                              height:200
                                           startTime:startTime
                                             endTime:endTime
                                               count:count
                                            callback:^(NSArray<QHVCEditThumbnailItem *> *thumbnails, QHVCEditThumbnailCallbackState state){}];
```

- 取消获取缩略图操作

```Objective-C
[self.thumbnailFactory cancelGettingVideoThumbnail];
```

- 释放缩略图对象

```Objective-C
[self.thumbnailFactory free];
```

#### 合成操作

- 初始化

```Objective-C
self.makerFactory = [[QHVCEditMaker alloc] initWithCommandFactory:self.commandFactory];
[self.makerFactory setMakerDelegate:self];

self.fileSavePath = [NSString stringWithFormat:@"%@%@", NSTemporaryDirectory(), @"edit.mp4"];
QHVCEditCommandMakeFile* command = [[QHVCEditCommandMakeFile alloc] initCommand:self.commandFactory];
[command setFilePath:self.fileSavePath];
[command addCommand];
```

- 开始合成

```Objective-C
[self.makerFactory makerStart];
```

- 取消合成

```Objective-C
[self.makerFactory makerStop];
```

- 释放

```Objective-C
[self.makerFactory free];
```

- 协议部分

```Objective-C
/**
 合成回调

 @param status 当前合成状态
 @param progress 当前合成进度（0-100）
 */
- (void)onMakerProcessing:(QHVCEditMakerStatus)status progress:(int)progress;
```

# API部分
## 指令
### 指令工厂初始化

```Objective-C
@interface QHVCEditCommandFactory : NSObject

- (instancetype)initCommandFactory; //初始化指令工厂
- (QHVCEditCommandError)freeCommandFactory; //释放指令工厂

//获取文件片段列表信息block (片段信息数组，片段总时长)
typedef void(^QHVCEditSegmentInfoBlock)(NSArray<QHVCEditSegmentInfo *>* segments, NSInteger totalDurationMs);
- (QHVCEditCommandError)getSegmentInfo:(QHVCEditSegmentInfoBlock)complete; //获取文件片段列表信息

@property (nonatomic, retain) QHVCEditOutputParams* defaultOutputParams; //默认输出样式
@property (nonatomic, readonly, assign) NSInteger factoryHandle;

@end
```

### 指令初始化

```Objective-C
@interface QHVCEditCommand : NSObject

- (instancetype)initCommand:(QHVCEditCommandFactory *)commandFactory; //创建指令
- (QHVCEditCommandError)addCommand; //添加指令

@property (nonatomic, readonly, assign) NSInteger commandId;  //指令ID

@end

@interface QHVCEditEditableCommand : QHVCEditCommand

- (QHVCEditCommandError)editCommand;   //修改已发送指令
- (QHVCEditCommandError)deleteCommand; //删除已发送指令

@end
```

### 指令操作

```Objective-C
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
@property (nonatomic, assign) int minHueAngle;                              //最小色调值(0-360)
@property (nonatomic, assign) int maxHueAngle;                              //最大色调值(0-360)
@property (nonatomic, assign) int extend;                                   //扩展（0-10）,在最大、最小色调区间外再扩展一定范围的过度区间，让抠图效果更自然
@property (nonatomic, assign) NSTimeInterval startTimestampMs;              //起始时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）
@property (nonatomic, assign) NSTimeInterval endTimestampMs;                //结束时间点，相对所有文件所在时间轴，开始时间和结束时间不能跨片段（单位：毫秒）

@end
```
## 实时预览

```Objective-C
/**
 初始化播放器

 @return 播放器对象
 */
- (instancetype)initPlayerWithCommandFactory:(QHVCEditCommandFactory *)commandFactory;


/**
 设置播放器代理对象

 @param delegate 代理对象
 @return 是否设置成功
 */
- (QHVCEditPlayerError)setPlayerDelegate:(id<QHVCEditPlayerDelegate>)delegate;


/**
 关闭播放器

 @return 是否关闭成功
 */
- (QHVCEditPlayerError)free;


/**
 播放器是否处于播放状态

 @return 是否处于播放状态
 */
- (BOOL)isPlayerPlaying;


/**
 设置预览画布

 @param preview 预览画布
 @return 是否设置成功
 */
- (QHVCEditPlayerError)setPlayerPreview:(UIView *)preview;


/**
 设置预览画布填充模式
 默认使用 QHVCEditPlayerPreviewFillMode_AspectFit

 @param mode 填充模式
 @return 是否设置成功
 */
- (QHVCEditPlayerError)setPreviewFillMode:(QHVCEditPlayerPreviewFillMode)mode;


/**
 设置预览画布填充背景色
 默认黑色

 @param color 填充背景色 (16进制值, ARGB)
 @return 是否设置成功
 */
- (QHVCEditPlayerError)setPreviewBackgroudColor:(NSString *)color;


/**
 重置播放器
 若播放器初始化之后有文件操作均需重置播放器（移序、删除、添加文件、添加背景音）

 @param seekTimestampMs 重置并将播放器跳转至某时间点
 @return 是否设置成功
 */
- (QHVCEditPlayerError)resetPlayer:(NSTimeInterval)seekTimestampMs;


/**
 刷新播放器
 若播放器初始化之后有效果添加需刷新播放器（字幕、贴纸、水印、滤镜、画质等）
 
 @return 是否设置成功
 */
- (QHVCEditPlayerError)refreshPlayer;

/**
 播放

 @return 是否调用成功
 */
- (QHVCEditPlayerError)playerPlay;


/**
 停止

 @return 是否调用成功
 */
- (QHVCEditPlayerError)playerStop;


/**
 跳转回调

 @param currentTime 当前跳转时间点（单位：毫秒）
 */
typedef void(^QHVCEditPlayerSeekCallback)(NSTimeInterval currentTime);

/**
 跳转至某个时间点

 @param time 跳转时间点（相对所有文件时间轴，单位：毫秒）
 @param forceRequest 是否强制请求
 @param block 跳转完成回调
 @return 是否调用成功
 */
- (QHVCEditPlayerError)playerSeekToTime:(NSTimeInterval)time forceRequest:(BOOL)forceRequest complete:(QHVCEditPlayerSeekCallback)block;


/**
 跳转至某个片段第一帧

 @param index 片段序号
 @param block 跳转完成回调
 @return 是否调用成功
 */
- (QHVCEditPlayerError)playerSeekToSegment:(NSInteger)index complete:(QHVCEditPlayerSeekCallback)block;


/**
 获取当前时间戳

 @return 当前时间戳
 */
- (NSTimeInterval)getCurrentTimestamp;


/**
 获取当前视频帧（带所有效果）
 
 @return 视频帧
 */
- (UIImage *)getCurrentFrame;


//播放器回调
@protocol QHVCEditPlayerDelegate <NSObject>
@optional

/**
 播放器错误回调

 @param error 错误类型
 @param info 详细错误信息
 */
- (void)onPlayerError:(QHVCEditPlayerError)error detailInfo:(QHVCEditPlayerErrorInfo)info;

/**
 播放完成回调
 */
- (void)onPlayerPlayComplete;

/**
 播放器第一帧已渲染
 */
- (void)onPlayerFirstFrameDidRendered;

@end

```

## 缩略图

```Objective-C
/**
 初始化缩略图生成器

 @return 缩略图生成器对象
 */
- (instancetype)initThumbnailFactory;


/**
 释放缩略图生成器
 
 @return 是否调用成功
 */
- (QHVCEditThumbnailError)free;


/**
 获取缩略图回调
 异步操作，可能会陆续回调缩略图

 @param thumbnails 缩略图数组
 @param state 当前状态
 */
typedef void(^QHVCEditThumbnailCallback)(NSArray<QHVCEditThumbnailItem*>* thumbnails, QHVCEditThumbnailCallbackState state);


/**
 获取缩略图

 @param filePath 文件物理路径
 @param width 缩略图宽度
 @param height 缩略图库高度
 @param startTimestampMs 起始时间点，相对当前文件（单位：毫秒）
 @param endTimestampMs 结束时间点，相对当前文件（单位：毫秒）
 @param count 期望获取视频帧数，会根据视频fps做调整，可能实际拿到视频帧数小于期望值
 @param block 获取缩略图回调
 @return 是否调用成功
 */
- (QHVCEditThumbnailError)getVideoThumbnailFromFile:(NSString *)filePath
                                     width:(int)width
                                    height:(int)height
                                 startTime:(NSTimeInterval)startTimestampMs
                                   endTime:(NSTimeInterval)endTimestampMs
                                     count:(int)count
                                  callback:(QHVCEditThumbnailCallback)block;


/**
 取消获取缩略图

 @return 是否调用成功
 */
- (QHVCEditThumbnailError)cancelGettingVideoThumbnail;
```

## 合成

```Objective-C
/**
 初始化合成器

 @param commandFactory 指令工厂
 @return 合成器实例对象
 */
- (instancetype)initWithCommandFactory:(QHVCEditCommandFactory *)commandFactory;


/**
 设置合成器代理对象

 @param delegate 代理对象
 @return 是否调用成功
 */
- (QHVCEditMakerError)setMakerDelegate:(id<QHVCEditMakerDelegate>)delegate;


/**
 关闭合成器

 @return 是否调用成功
 */
- (QHVCEditMakerError)free;


/**
 开始合成

 @return 是否调用成功
 */
- (QHVCEditMakerError)makerStart;


/**
 停止合成，用于打断操作

 @return 是否调用成功
 */
- (QHVCEditMakerError)makerStop;

```

## 日志
``` Objective-C
typedef NS_ENUM(NSUInteger, QHVCEditLogLevel)
{
    QHVCEditLogLevel_Error,  //仅包含错误
    QHVCEditLogLevel_Warn,   //错误+警告
    QHVCEditLogLevel_Info,   //错误+警告+状态信息
    QHVCEditLogLevel_Debug,  //错误+警告+状态信息+调试信息
};

@interface QHVCEditLog : NSObject

/**
 设置日志级别

 @param level 日志级别
 */
+ (void)setLogLevel:(QHVCEditLogLevel)level;

@end
```

### SDK集成注意事项

```
QHVCEditKit.framework是动态库，一定要在TARGETS->General->Embed Binaries下引入
```

```
所有操作均依赖指令集，初始化播放器、初始化合成对象前，请确保已初始化指令工厂且正确配置输出参数。
```

### 错误码说明

QHVCEditOutputError（输出参数）

| 状态码 | 字段                            | 注释    |
|:-----:|--------------------------------|---------|
|   1   | QHVCEditOutputError_NoError    | 无错误   |
|   2   | QHVCEditOutputError_ParamError | 参数错误 |

QHVCEditCommandError（指令操作）

| 状态码 | 字段                               | 注释       |
|:-----:|-----------------------------------|------------|
|   1   | QHVCEditCommandError_NoError      | 无错误      |
|   2   | QHVCEditCommandError_ParamError   | 参数错误    |
|   3   | QHVCEditCommandError_FactoryError | 指令工厂错误 |
|   4   | QHVCEditCommandError_SendError    | 指令发送错误 |

QHVCEditPlayerError（播放器基本错误类型）

| 状态码 | 字段                             | 注释       |
|:-----:|---------------------------------|------------|
|   1   | QHVCEditPlayerError_NoError     | 无错误      |
|   2   | QHVCEditPlayerError_ParamError  | 参数错误    |
|   3   | QHVCEditPlayerError_InitError   | 初始化失败   |
|   4   | QHVCEditPlayerError_StatusError | 播放状态错误 |

QHVCEditPlayerErrorInfo（播放器详细错误类型）

| 状态码 | 字段                                         | 注释           |
|:-----:|---------------------------------------------|----------------|
|   1   | QHVCEditPlayerErrorInfo_NoError             | 无错误          |
|   2   | QHVCEditPlayerErrorInfo_CommandFactoryError | edit handle错误 |
|   3   | QHVCEditPlayerErrorInfo_PlayerHandleError   | 播放器handle错误 |

QHVCEditThumbnailError（缩略图基本错误类型）

| 状态码 | 字段                                 | 注释         |
|:-----:|-------------------------------------|--------------|
|   1   | QHVCEditThumbnailError_NoError      | 无错误        |
|   2   | QHVCEditThumbnailError_InitError    | 初始化失败     |
|   3   | QHVCEditThumbnailError_processError | 获取缩略图错误 |
|   4   |  QHVCEditThumbnailError_ParamError  | 参数错误      |

QHVCEditMakerError （合成基本错误类型）

| 状态码 | 字段                             | 注释       |
|:-----:|---------------------------------|------------|
|   1   | QHVCEditMakerError_NoError      | 无错误      |
|   2   | QHVCEditMakerError_FactoryError | 指令工厂错误 |
|   3   | QHVCEditMakerError_InitError    | 初始化失败   |


