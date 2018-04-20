//
//  QHVCInteractiveKit.h
//  QHVCInteractiveKit
//
//  Created by yangkui on 2018/1/25.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

#pragma mark - 枚举对象定义 -

typedef NS_ENUM(NSInteger, QHVCITLLogLevel){//日志等级
    QHVCITL_LOG_LEVEL_TRACE = 0,
    QHVCITL_LOG_LEVEL_DEBUG = 1,
    QHVCITL_LOG_LEVEL_INFO  = 2,
    QHVCITL_LOG_LEVEL_WARN  = 3,
    QHVCITL_LOG_LEVEL_ERROR = 4,
    QHVCITL_LOG_LEVEL_ALARM = 5,
    QHVCITL_LOG_LEVEL_FATAL = 6,
    QHVCITL_LOG_LEVEL_NONE  = 7,
};

typedef NS_ENUM(NSInteger, QHVCITLWarningCode) {//警告代码，代码中可以忽略该消息
    QHVCITL_Warn_InvalidView = 8,
    QHVCITL_Warn_InitVideo = 16,
    QHVCITL_Warn_Pending = 20,
    QHVCITL_Warn_NoAvailableChannel = 103,
    QHVCITL_Warn_LookupChannelTimeout = 104,
    QHVCITL_Warn_LookupChannelRejected = 105,
    QHVCITL_Warn_OpenChannelTimeout = 106,
    QHVCITL_Warn_OpenChannelRejected = 107,
    QHVCITL_Warn_SwitchLiveVideoTimeout = 111,
    // sdk:vos, callmanager, peermanager: 100~1000
    QHVCITL_Warn_SetClientRoleTimeout = 118,
    QHVCITL_Warn_SetClientRoleNotAuthorized = 119,
    QHVCITL_Warn_AudioMixingOpenError = 701,
    
    QHVCITL_Warn_Adm_RuntimePlayoutWarning = 1014,
    QHVCITL_Warn_Adm_RuntimeRecordingWarning = 1016,
    QHVCITL_Warn_Adm_RecordAudioSilence = 1019,
    QHVCITL_Warn_Adm_PlaybackMalfunction = 1020,
    QHVCITL_Warn_Adm_RecordMalfunction = 1021,
    QHVCITL_Warn_Adm_Interruption = 1025,
    QHVCITL_Warn_Adm_RouteChange = 1026,
    QHVCITL_Warn_Apm_Howling = 1051,
};

typedef NS_ENUM(NSInteger, QHVCITLErrorCode) {//错误码，代码中必须处理该消息
    QHVCITL_Error_NoError = 0,
    QHVCITL_Error_Failed = 1,
    QHVCITL_Error_InvalidArgument = 2,
    QHVCITL_Error_NotReady = 3,
    QHVCITL_Error_NotSupported = 4,
    QHVCITL_Error_Refused = 5,
    QHVCITL_Error_BufferTooSmall = 6,
    QHVCITL_Error_NotInitialized = 7,
    QHVCITL_Error_InvalidView = 8,
    QHVCITL_Error_NoPermission = 9,
    QHVCITL_Error_TimedOut = 10,
    QHVCITL_Error_Canceled = 11,
    QHVCITL_Error_TooOften = 12,
    QHVCITL_Error_BindSocket = 13,
    QHVCITL_Error_NetDown = 14,
    QHVCITL_Error_NoBufs = 15,
    QHVCITL_Error_InitVideo = 16,
    QHVCITL_Error_JoinChannelRejected = 17,
    QHVCITL_Error_LeaveChannelRejected = 18,
    QHVCITL_Error_AlreadyInUse = 19,
    
    QHVCITL_Error_InvalidAppId = 101,
    QHVCITL_Error_InvalidChannelName = 102,
    QHVCITL_Error_ChannelKeyExpired = 109,
    QHVCITL_Error_InvalidChannelKey = 110,
    QHVCITL_Error_ConnectionLost = 112,
    QHVCITL_Error_NotInChannel = 113,
    QHVCITL_Error_SizeTooLarge = 114,
    QHVCITL_Error_BitrateLimit = 115,
    QHVCITL_Error_TooManyDataStreams = 116,
    QHVCITL_Error_DecryptionFailed = 120,
    
    QHVCITL_Error_LoadMediaEngine = 1001,
    QHVCITL_Error_StartCall = 1002,
    QHVCITL_Error_StartCamera = 1003,
    QHVCITL_Error_StartVideoRender = 1004,
    QHVCITL_Error_Adm_GeneralError = 1005,
    QHVCITL_Error_Adm_JavaResource = 1006,
    QHVCITL_Error_Adm_SampleRate = 1007,
    QHVCITL_Error_Adm_InitPlayout = 1008,
    QHVCITL_Error_Adm_StartPlayout = 1009,
    QHVCITL_Error_Adm_StopPlayout = 1010,
    QHVCITL_Error_Adm_InitRecording = 1011,
    QHVCITL_Error_Adm_StartRecording = 1012,
    QHVCITL_Error_Adm_StopRecording = 1013,
    QHVCITL_Error_Adm_RuntimePlayoutError = 1015,
    QHVCITL_Error_Adm_RuntimeRecordingError = 1017,
    QHVCITL_Error_Adm_RecordAudioFailed = 1018,
    QHVCITL_Error_Adm_Play_Abnormal_Frequency = 1020,
    QHVCITL_Error_Adm_Record_Abnormal_Frequency = 1021,
    QHVCITL_Error_Adm_Init_Loopback  = 1022,
    QHVCITL_Error_Adm_Start_Loopback = 1023,
    
    // VDM error code starts from 1500
    QHVCITL_Error_Vdm_Camera_Not_Authorized = 1501,
    
    // VCM error code starts from 1600
    QHVCITL_Error_Vcm_Unknown_Error = 1600,
    QHVCITL_Error_Vcm_Encoder_Init_Error = 1601,
    QHVCITL_Error_Vcm_Encoder_Encode_Error = 1602,
    QHVCITL_Error_Vcm_Encoder_Set_Error = 1603,
    
    // 水熊 error code starts from 4000
    QHVCITL_Error_Argument_Missing = 4000,//参数缺失
    QHVCITL_Error_Internal_Exceptional = 4101,//服务内部异常
    QHVCITL_Error_Request_Expired = 4102,//请求已过期
    QHVCITL_Error_Authentication = 4103,//签名验证失败
    QHVCITL_Error_Invalid_Service = 4201,//渠道ID服务不存在
    QHVCITL_Error_Service_Unavailable = 4202,//渠道ID服务未开通
    QHVCITL_Error_Create_Room = 4301,//创建房间失败
    QHVCITL_Error_Join_Room = 4302,//加入房间失败
    QHVCITL_Error_Quit_Room = 4303,//退出房间失败
    QHVCITL_Error_Get_Room_Info = 4304,//获取房间信息失败
    QHVCITL_Error_Update_Member_Attribute = 4305,//更新成员属性失败
    QHVCITL_Error_Update_Room_Attribute = 4306,//更新房间属性失败
    QHVCITL_Error_Update_Heartbeat = 4307,//心跳更新失败
    QHVCITL_Error_Room_Already_Exists = 4308,//房间已存在
    QHVCITL_Error_Invalid_Server_Name = 4309,//渠道ID不可用
    QHVCITL_Error_Generate_RTC_CTX = 4310,//生成连麦信息失败
    QHVCITL_Error_Unavailable_Configuration = 4311,//没有可用的连麦配置
    QHVCITL_Error_Not_Available_Live_Setting = 4312,//没有可用的直播配置
    QHVCITL_Error_Already_Join_Room = 4313,//已加入过房间，不能重复加入
    
    // 混流错误
    QHVCITL_Error_MixStream_Input_Not_Exist = -150,//混流的输入流不存在
    QHVCITL_Error_MixStream_Failed = -151,//混流失败
    QHVCITL_Error_MixStream_Stop_Failed = -152,//停止混流失败
    QHVCITL_Error_MixStream_Input_Parameter_Error = -153,//输入参数错误
    QHVCITL_Error_MixStream_Output_Parameter_Error = -154,//输出参数错误
    QHVCITL_Error_MixStream_Input_Ratio_Error = -155,//输入分辨率格式错误
    QHVCITL_Error_MixStream_Output_Ratio_Error = -156,//输出分辨率格式错误
    QHVCITL_Error_MixStream_Not_Open = -157,//混流没开
    
    QHVCITL_Error_TempBroken = -200,//直播临时中断
    QHVCITL_Error_FatalError = -201,//直播遇到严重的问题
    QHVCITL_Error_CreateStreamError = -202,//创建直播流失败
    QHVCITL_Error_FetchStreamError = -203,//获取流信息失败
    QHVCITL_Error_NoStreamError = -204,//无流信息
    QHVCITL_Error_LogicServerNetWrokError  = -205,//逻辑服务器网络错误
    QHVCITL_Error_DNSResolveError = -206,//DNS 解释失败
    QHVCITL_Error_NotLoginError = -207,//未登录
    QHVCITL_Error_UnknownError = -208,//未知错误
    QHVCITL_Error_PublishBadNameError = -209,
    QHVCITL_Error_HttpDNSResolveError = -210,

};

typedef NS_ENUM(NSInteger, QHVCITLChannelProfile) {//频道属性
    QHVCITL_ChannelProfile_Communication = 0,
    QHVCITL_ChannelProfile_LiveBroadcasting = 1,
    QHVCITL_ChannelProfile_Game = 2,
};

typedef NS_ENUM(NSInteger, QHVCITLClientRole) {//角色属性
    QHVCITL_ClientRole_Broadcaster = 1,
    QHVCITL_ClientRole_Audience = 2,
};

typedef NS_ENUM(NSInteger, QHVCITLVideoProfile) {//视频属性
                                             // res       fps  kbps
    QHVCITL_VideoProfile_240P_3 = 22,        // 240x240   15   140
    QHVCITL_VideoProfile_240P_4 = 23,        // 424x240   15   220
    QHVCITL_VideoProfile_360P = 30,          // 640x360   15   400
    QHVCITL_VideoProfile_360P_3 = 32,        // 360x360   15   260
    QHVCITL_VideoProfile_360P_4 = 33,        // 640x360   15   600
    QHVCITL_VideoProfile_360P_6 = 35,        // 360x360   15   400
    QHVCITL_VideoProfile_360P_9 = 38,        // 640x360   15   800
    QHVCITL_VideoProfile_480P_3 = 42,        // 480x480   15   400
    QHVCITL_VideoProfile_480P_6 = 45,        // 480x480   30   600
    QHVCITL_VideoProfile_480P_8 = 47,        // 848x480   15   610
    QHVCITL_VideoProfile_480P_9 = 48,        // 848x480   30   930
    QHVCITL_VideoProfile_720P = 50,          // 1280x720  15   1130
    QHVCITL_VideoProfile_720P_3 = 52,        // 1280x720  30   1710
    QHVCITL_VideoProfile_504P_1 = 100,       // 896x504   15   800
    QHVCITL_VideoProfile_576P_1 = 110,       // 1024x576  15   900
    QHVCITL_VideoProfile_DEFAULT = QHVCITL_VideoProfile_360P,
};

typedef NS_ENUM(NSUInteger, QHVCITLQuality) {//网路质量
    QHVCITL_Quality_Unknown = 0,
    QHVCITL_Quality_Excellent = 1,
    QHVCITL_Quality_Good = 2,
    QHVCITL_Quality_Poor = 3,
    QHVCITL_Quality_Bad = 4,
    QHVCITL_Quality_VBad = 5,
    QHVCITL_Quality_Down = 6,
};

typedef NS_ENUM(NSUInteger, QHVCITLUserOfflineReason) {//用户离线原因
    QHVCITL_UserOffline_Quit = 0,//用户主动离开
    QHVCITL_UserOffline_Dropped = 1,//因过长时间收不到对方数据包，超时掉线。注意：由于SDK使用的是不可靠通道，也有可能对方主动离开本方没收到对方离开消息而误判为超时掉线
    QHVCITL_UserOffline_BecomeAudience = 2,//当用户身份从主播切换为观众时触发,如果播放使用的业务自己管理拉取逻辑，将不会触发这个状态。
};

typedef NS_ENUM(NSInteger, QHVCITLVideoStreamType) {//视频流类型
    QHVCITL_VideoStream_High = 0,//高码率
    QHVCITL_VideoStream_Low = 1,//低码率
};

typedef NS_ENUM(NSInteger, QHVCITLAudioOutputRouting){//声音输出设备
    QHVCITL_AudioOutputRouting_Default = -1,
    QHVCITL_AudioOutputRouting_Headset = 0,
    QHVCITL_AudioOutputRouting_Earpiece = 1,
    QHVCITL_AudioOutputRouting_HeadsetNoMic = 2,
    QHVCITL_AudioOutputRouting_Speakerphone = 3,
    QHVCITL_AudioOutputRouting_Loudspeaker = 4,
    QHVCITL_AudioOutputRouting_HeadsetBluetooth = 5
};

typedef NS_ENUM(NSInteger, QHVCITLRtmpStreamLifeCycle){//推流生命周期属性
    QHVCITL_RtmpStream_LifeCycle_Bind_To_Channel = 1,//绑定到频道
    QHVCITL_RtmpStream_LifeCycle_Bind_To_Owner = 2,//绑定到主播
};

typedef NS_ENUM(NSUInteger, QHVCITLRenderMode) {//画面渲染模式
    QHVCITL_Render_ScaleAspectFill    = 1,//等比缩放填充整View，可能有部分被裁减
    QHVCITL_Render_ScaleAspectFit     = 2,//等比缩放，可能有黑边
    QHVCITL_Render_ScaleToFill        = 3,//填充整个View
};

typedef NS_ENUM(NSUInteger, QHVCITLAudioFrameType) {//音频格式类型
    QHVCITL_AudioFrame_type_PCM16 = 0,  //PCM 16bit 小端
};

typedef NS_ENUM(NSUInteger, QHVCITLVideoFrameType) {//视频格式信息
    QHVCITL_VideoFrame_type_YUV420 = 0,  //YUV 420 format
};

typedef NS_ENUM(NSInteger, QHVCITLDataCollectMode) {//数据采集方式
    QHVCITLDataCollectModeSDK         =  1,//SDK自采集
    QHVCITLDataCollectModeUser        =  2,//用户采集
};


#pragma mark - 数据结构定义 -
/**
 视频画布信息
 */
@interface QHVCITLVideoCanvas : NSObject
@property (strong, nonatomic, nullable) UIView* view;//视频显示视窗
@property (assign, nonatomic) QHVCITLRenderMode renderMode; // 渲染模式
@property (strong, nonatomic, nonnull) NSString* uid; // 用户ID
@end

/**
 音视频统计信息，包括瞬时值、累计值
 */
@interface QHVCITLStatistics : NSObject
@property (strong, nonatomic, nonnull) NSString* uid;//用户ID
@property (assign, nonatomic) NSUInteger duration;//通话时长，累计值
@property (assign, nonatomic) NSUInteger txBytes;//发送字节数，累计值
@property (assign, nonatomic) NSUInteger rxBytes;//接收字节数，累计值
@property (assign, nonatomic) NSUInteger txAudioKBitrate;//瞬时值
@property (assign, nonatomic) NSUInteger rxAudioKBitrate;//瞬时值
@property (assign, nonatomic) NSUInteger txVideoKBitrate;//瞬时值
@property (assign, nonatomic) NSUInteger rxVideoKBitrate;//瞬时值
@end

/**
 本地发送视频统计信息
 */
@interface QHVCITLLocalVideoStats : NSObject
@property (assign, nonatomic) NSUInteger sentBitrate;//（上次统计后）发送的字节数
@property (assign, nonatomic) NSUInteger sentFrameRate;//（上次统计后）发送的帧数
@end

/**
 接收远端视频统计信息
 */
@interface QHVCITLRemoteVideoStats : NSObject
@property (strong, nonatomic, nonnull) NSString* uid;//用户ID，指定是哪个用户的视频流
@property (assign, nonatomic) NSUInteger delay;//延时(毫秒)
@property (assign, nonatomic) NSUInteger width;//视频流宽（像素）
@property (assign, nonatomic) NSUInteger height;//视频流高（像素）
@property (assign, nonatomic) NSUInteger receivedBitrate;//（上次统计后）接收到的码率(kbps)
@property (assign, nonatomic) NSUInteger receivedFrameRate;//（上次统计后）接收帧率(fps)
@property (assign, nonatomic) QHVCITLVideoStreamType rxStreamType;//流的质量
@end

/**
 音量信息
 */
@interface QHVCITLAudioVolumeInfo : NSObject
@property (strong, nonatomic, nonnull) NSString* uid;
@property (assign, nonatomic) NSUInteger volume;
@end

/**
 音频数据格式定义
 */
@interface QHVCITLAudioFrame : NSObject
@property (assign, nonatomic) QHVCITLAudioFrameType type;//声音数据格式
@property (assign, nonatomic) NSInteger samples;//该帧的样本数量
@property (assign, nonatomic) NSInteger bytesPerSample;//每个样本的字节数: PCM (16位)含两个字节
@property (assign, nonatomic) NSInteger channels;//频道数量
@property (assign, nonatomic) NSInteger sampleRate;//采样率
@property (assign, nonatomic) NSInteger bufferLength;//数据长度
@property (assign, nonatomic, nullable) void* buffer;//数据缓冲区
@property (assign, nonatomic) NSInteger renderTimeMs;//渲染音频流的时间戳。用户在进行音频流渲染时使用该时间戳同步音频流的渲染。
@end

/**
 视频数据格式定义
 */
@interface QHVCITLVideoFrame : NSObject
@property(assign, nonatomic) QHVCITLVideoFrameType type;//视频格式
@property(assign, nonatomic) NSInteger width;//视频宽
@property(assign, nonatomic) NSInteger height;//视频高
@property(assign, nonatomic) NSInteger yStride;//stride of Y data buffer
@property(assign, nonatomic) NSInteger uStride;//stride of U data buffer
@property(assign, nonatomic) NSInteger vStride;//stride of V data buffer
@property(assign, nonatomic, nullable) void* yBuffer;//Y data buffer
@property(assign, nonatomic, nullable) void* uBuffer;//U data buffer
@property(assign, nonatomic, nullable) void* vBuffer;//V data buffer
@property(assign, nonatomic) NSInteger rotation;// rotation of this frame (0, 90, 180, 270)
@property(assign, nonatomic) NSInteger renderTimeMs;//渲染的时间戳
@end

/**
 任意一路视频合流位置定义
 */
@interface QHVCITLVideoCompositingRegion : NSObject
@property (assign, nonatomic, nonnull) NSString* uid;//待显示在该区域的主播用户uid，唯一
@property (assign, nonatomic) double x;//x[0.0,1.0]: 屏幕里该区域的横坐标
@property (assign, nonatomic) double y;//y[0.0,1.0]: 屏幕里该区域的纵坐标
@property (assign, nonatomic) double width;//w[0.0, 1.0]:该区域的实际宽度
@property (assign, nonatomic) double height;//h[0.0, 1.0]:该区域的实际高度
@property (assign, nonatomic) NSInteger zOrder; //zOrder[0, 100], 0表示该区域图像位于最下层，而100表示该区域图像位于最上层。
@property (assign, nonatomic) double alpha; //alpha[0, 1.0] 0表示图像为透明的，1表示图像为完全不透明的
@property (assign, nonatomic) QHVCITLRenderMode renderMode;//QHVCITL_Render_ScaleAspectFill: 经过裁减的,QHVCITL_Render_ScaleAspectFit: 缩放到合适大小
@end

/**
 房间视频合流布局定义
 */
@interface QHVCITLVideoCompositingLayout : NSObject
@property (assign, nonatomic) NSInteger canvasWidth;//整个屏幕(画布)的宽度
@property (assign, nonatomic) NSInteger canvasHeight;//整个屏幕(画布)的高度
@property (copy, nonatomic, nullable) NSString* backgroundColor;//背景颜色，e.g. "#c0c0c0"
@property (retain, nonatomic, nonnull) NSMutableArray<QHVCITLVideoCompositingRegion *>* regions; //频道内每位主播在屏幕上均可以有一个区域显示自己的头像或视频。
@end

/**
 合流转推配置
 */
@interface QHVCITLMixerPublisherConfiguration : NSObject
@property (copy, nonatomic, nullable) NSString* publishUrl;//合流推流地址
@property (assign, nonatomic) QHVCITLRtmpStreamLifeCycle lifeCycle;//流的生命周期属性
@property (assign, nonatomic) NSInteger width;//视频分辨率宽
@property (assign, nonatomic) NSInteger height;//视频分辨率高
@property (assign, nonatomic) NSInteger framerate;//帧率
@property (assign, nonatomic) NSInteger bitrate;//码率
@property (nonatomic, assign) NSInteger gop;//视频最大I帧间隔
@property (nonatomic, strong, nullable) NSString* audioFormat;//声音编码格式，AAC
@property (nonatomic, assign) NSInteger audioSample;//声音采样率
@property (nonatomic, assign) NSInteger audioBitrate;//声音码率
@property (nonatomic, assign) NSInteger audioChannel;//声音声道数

- (NSDictionary * _Nonnull)transformToDictionary;

@end


#pragma mark - 定义互动直播代理回调 -
@class QHVCInteractiveKit;
@protocol QHVCInteractiveDelegate <NSObject>
@optional
/**
 发生警告回调
 该回调方法表示SDK运行时出现了（网络或媒体相关的）警告。通常情况下，SDK上报的警告信息应用程序可以忽略，SDK会自动恢复。
 
 @param engine 引擎对象
 @param warningCode 警告代码
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didOccurWarning:(QHVCITLWarningCode)warningCode;


/**
 本地推流端发生错误回调
 该回调方法表示SDK运行时出现了（网络或媒体相关的）错误。通常情况下，SDK上报的错误意味着SDK无法自动恢复，需要应用程序干预或提示用户。比如启动通话失败时，SDK会上报QHVCITL_Error_StartCall(1002)错误。应用程序可以提示用户启动通话失败，并调用leaveChannel退出频道。
 
 @param engine 引擎对象
 @param errorCode 错误代码
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didOccurError:(QHVCITLErrorCode)errorCode;


/**
 加载互动直播引擎数据成功回调
 该回调方法表示SDK加载引擎数据成功。该回调成功后，业务可以进行一系列参数的设置，之后调用joinChannel以及后续操作。
 
 @param engine 引擎对象
 @param dataDict 参数字典，将会返回业务所需的必要信息
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didLoadEngineData:(nullable NSDictionary *)dataDict;

/**
 音量提示回调
 提示谁在说话及其音量，默认禁用。可通过enableAudioVolumeIndication方法设置。
 
 @param engine 引擎对象
 @param speakers 说话者（数组）。每个speaker()：uid: 说话者的用户ID,volume：说话者的音量（0~255）
 @param totalVolume 混音后的）总音量（0~255）
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine reportAudioVolumeIndicationOfSpeakers:(nullable NSArray *)speakers totalVolume:(NSInteger)totalVolume;


/**
 本地首帧视频显示回调
 提示第一帧本地视频画面已经显示在屏幕上。
 
 @param engine 引擎对象
 @param size 视频流尺寸（宽度和高度）
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine firstLocalVideoFrameWithSize:(CGSize)size;


/**
 远端首帧视频接收解码回调
 提示已收到第一帧远程视频流并解码。
 
 @param engine 引擎对象
 @param uid 用户ID，指定是哪个用户的视频流
 @param size 视频流尺寸（宽度和高度）
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine firstRemoteVideoDecodedOfUid:(nonnull NSString *)uid size:(CGSize)size;


/**
 本地或远端用户更改视频大小的事件

 @param engine 引擎对象
 @param uid 用户ID
 @param size 视频新Size
 @param rotation 视频新的旋转角度
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine videoSizeChangedOfUid:(nonnull NSString *)uid size:(CGSize)size rotation:(NSInteger)rotation;


/**
 远端首帧视频显示回调
 提示第一帧远端视频画面已经显示在屏幕上。
 如果是主播推混流，这里需要在回调里面强制更新一下混流布局配置:
 setVideoCompositingLayout:(QHVCITLVideoCompositingLayout*)layout;
 
 @param engine 引擎对象
 @param uid 用户ID，指定是哪个用户的视频流
 @param size 视频流尺寸（宽度和高度）
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine firstRemoteVideoFrameOfUid:(nonnull NSString *)uid size:(CGSize)size;



/**
 用户加入回调
 提示有用户加入了频道。如果该客户端加入频道时已经有人在频道中，SDK也会向应用程序上报这些已在频道中的用户。
 
 @param engine 引擎对象
 @param uid 用户ID，如果joinChannel中指定了uid，则此处返回该ID；否则使用连麦服务器自动分配的ID。
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didJoinedOfUid:(nonnull NSString *)uid;


/**
 某个用户离线回调
 提示有用户离开了频道（或掉线）。
 
 @param engine 引擎对象
 @param uid 用户ID
 @param reason 离线原因
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didOfflineOfUid:(nonnull NSString *)uid reason:(QHVCITLUserOfflineReason)reason;


/**
 用户音频静音回调
 提示有用户用户将通话静音/取消静音。
 
 @param engine 引擎对象
 @param muted Yes:静音, No:取消静音
 @param uid 用户ID
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didAudioMuted:(BOOL)muted byUid:(nonnull NSString *)uid;


/**
 用户停止/重新发送视频回调
 提示有其他用户暂停发送/恢复发送其视频流。
 
 @param engine 引擎对象
 @param muted Yes：该用户已暂停发送其视频流 No：该用户已恢复发送其视频流
 @param uid 用户ID
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didVideoMuted:(BOOL)muted byUid:(nonnull NSString *)uid;


/**
 音频路由改变

 @param engine 引擎对象
 @param routing 新的输出设备
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didAudioRouteChanged:(QHVCITLAudioOutputRouting)routing;


/**
 用户启用/关闭视频回调
 提示有其他用户启用/关闭了视频功能。关闭视频功能是指该用户只能进行语音通话，不能显示、发送自己的视频，也不能接收、显示别人的视频。
 
 @param engine 引擎对象
 @param enabled Yes：该用户已启用了视频功能 No：该用户已关闭了视频功能
 @param uid 用户ID
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didVideoEnabled:(BOOL)enabled byUid:(nonnull NSString *)uid;


/**
 本地视频统计回调
 报告更新本地视频统计信息，该回调方法每两秒触发一次。
 
 @param engine 引擎对象
 @param stats sentBytes（上次统计后）发送的字节数 sentFrames（上次统计后）发送的帧数
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine localVideoStats:(nonnull QHVCITLLocalVideoStats *)stats;


/**
 远端视频统计回调
 报告更新远端视频统计信息，该回调方法每两秒触发一次。
 
 @param engine 引擎对象
 @param stats 统计信息
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine remoteVideoStats:(nonnull QHVCITLRemoteVideoStats *)stats;


/**
 摄像头启用回调
 提示已成功打开摄像头，可以开始捕获视频。
 
 @param engine 引擎对象
 */
- (void)interactiveEngineCameraDidReady:(nonnull QHVCInteractiveKit *)engine;


/**
 视频功能停止回调
 提示视频功能已停止。应用程序如需在停止视频后对view做其他处理（比如显示其他画面），可以在这个回调中进行。
 
 @param engine 引擎对象
 */
- (void)interactiveEngineVideoDidStop:(nonnull QHVCInteractiveKit *)engine;


/**
 本地网络连接中断回调
 在SDK和服务器失去了网络连接时，触发该回调。失去连接后，除非APP主动调用leaveChannel，SDK会一直自动重连。
 
 @param engine 引擎对象
 */
- (void)interactiveEngineConnectionDidInterrupted:(nonnull QHVCInteractiveKit *)engine;


/**
 本地网络连接丢失回调
 在SDK和服务器失去了网络连接后，会触发interactiveEngineConnectionDidInterrupted回调，并自动重连。在一定时间内（默认10秒）如果没有重连成功，触发interactiveEngineConnectionDidLost回调。除非APP主动调用leaveChannel，SDK仍然会自动重连。
 
 @param engine 引擎对象
 */
- (void)interactiveEngineConnectionDidLost:(nonnull QHVCInteractiveKit *)engine;


/**
 加入频道成功回调
 该回调方法表示该客户端成功加入了指定的频道。
 
 @param engine 引擎对象
 @param channel 频道名
 @param uid 用户ID
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didJoinChannel:(nonnull NSString *)channel withUid:(nonnull NSString *)uid;


/**
 重新加入频道回调
 有时候由于网络原因，客户端可能会和服务器失去连接，SDK会进行自动重连，自动重连成功后触发此回调方法，提示有用户重新加入了频道，且频道ID和用户ID均已分配。
 
 @param engine 引擎对象
 @param channel 频道名
 @param uid 用户ID
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didRejoinChannel:(nonnull NSString *)channel withUid:(nonnull NSString *)uid;


/**
 统计数据回调
 该回调定期上报Interactive Engine的运行时的状态，每两秒触发一次。
 
 @param engine 引擎对象
 @param stats 统计值
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine reportStats:(nonnull QHVCITLStatistics *)stats;


/**
 用户主动离开频道回调
 
 @param engine 引擎对象
 @param stats 本次通话数据统计，包括时长、发送和接收数据量等
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine didLeaveChannelWithStats:(nullable QHVCITLStatistics *)stats;


/**
 语音质量回调
 在通话中，该回调方法每两秒触发一次，报告当前通话的（嘴到耳）音频质量。
 
 @param engine 引擎对象
 @param uid 用户ID
 @param quality 声音质量评分
 @param delay 延迟（毫秒）
 @param lost 丢包率（百分比）
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine audioQualityOfUid:(nonnull NSString *)uid quality:(QHVCITLQuality)quality delay:(NSUInteger)delay lost:(NSUInteger)lost;


/**
 频道内网络质量报告回调
 该回调定期触发，向APP报告频道内通话中用户当前的上行、下行网络质量。
 
 @param engine 引擎对象
 @param uid 用户ID。表示该回调报告的是持有该ID的用户的网络质量。当uid为0时，返回的是本地用户的网络质量。当前版本仅报告本地用户的网络质量。
 @param txQuality 该用户的上行网络质量。
 @param rxQuality 该用户的下行网络质量。
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine networkQuality:(nonnull NSString *)uid txQuality:(QHVCITLQuality)txQuality rxQuality:(QHVCITLQuality)rxQuality;


/**
 抓取视频截图回调
 该回调方法由takeStreamSnapshot触发，返回的是对应uid当前流的图像
 
 @param engine 引擎对象
 @param img 截图对象
 @param uid 用户ID
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine takeStreamSnapshot:(nonnull CGImageRef)img uid:(nonnull NSString *)uid;

@end


/**
 视频输出回调
 */
@protocol QHVCInteractiveVideoFrameDelegate <NSObject>
@optional

/**
 创建一个CVPixelBufferRef对象的内存地址给SDK，接收到的图像将会存放到这个对象上，然后调用renderPixelBuffer方法进行通知
 
 @param engine 引擎对象
 @param width 视频宽
 @param height 视频高
 @param stride stride值
 */
- (nonnull CVPixelBufferRef)interactiveEngine:(nonnull QHVCInteractiveKit *)engine createInputBufferWithWidth:(int)width height:(int)height stride:(int)stride;

/**
 远端视频数据拷贝完毕后进行回调，通知业务进行渲染
 
 @param engine 引擎对象
 @param uid 用户ID
 @param pixelBuffer 视频对象，数据为一个CVPixelBufferRef对象
 */
- (void)interactiveEngine:(nonnull QHVCInteractiveKit *)engine renderPixelBuffer:(nonnull NSString *)uid pixelBuffer:(nonnull CVPixelBufferRef)pixelBuffer;

@end

/**
 音频输出回调
 */
@protocol QHVCInteractiveAudioFrameDelegate <NSObject>
@optional

/**
 该方法获取本地采集的音频数据，可以在此时机处理前置声音效果。
 
 @param audioFrame 声音数据
 */
- (void)onRecordLocalAudioFrame:(nonnull QHVCITLAudioFrame *)audioFrame;

/**
 该方法获取上行、下行所有数据混音后的数据。
 
 @param audioFrame 声音数据
 */
- (void)onMixedAudioFrame:(nonnull QHVCITLAudioFrame *)audioFrame;

@end


#pragma mark - 互动直播实现方法 -
@interface QHVCInteractiveKit : NSObject
#pragma mark - 定义全局静态方法
/**
 获取SDK版本号
 
 @return SDK版本号
 */
+ (nonnull NSString *) getVersion;


/**
 开启日志
 
 @param level 日志等级
 */
+ (void) openLogWithLevel:(QHVCITLLogLevel)level;


/**
 设置日志输出callback
 
 @param callback 回调block
 */
+ (void) setLogOutputCallBack:(void(^_Nullable)(int loggerID, QHVCITLLogLevel level, const char * _Nullable data))callback;


/**
 获取实例方法

 @return 返回静态实例对象
 */
+ (nonnull instancetype) sharedInstance;

/**
 销毁引擎实例
 */
+ (void)destory;


#pragma mark - 定义通用方法

/**
 设置公共业务信息
 该方法需要在所有实例方法之前调用，用于设置业务所需要的一些必要信息，便于业务区分、统计使用。
 
 @param channelId 渠道Id，用于区分公司或部门下拥有多款应用
 @param appKey 平台为每个应用分配的唯一ID
 @param userSign 用户签名
 @return 0 成功, 非0 表示失败。
 */
- (int) setPublicServiceInfo:(nonnull NSString *)channelId
                      appKey:(nonnull NSString *)appKey
                    userSign:(nonnull NSString *)userSign;


/**
 加载互动直播引擎数据，进行准备工作。
 调用该方法前，必须先调用setPublicServiceInfo进行业务信息准备，在该方法必须调用后才能调用其它实例方法。该方法是异步执行，待回调执行之后才能执行其它实例方法。若加载成功，SDK将会触发didLoadEngineData回调，然后，业务可以进行一系列参数的设置，之后调用joinChannel以及后续操作。若加载失败，SDK将会触发didOccurError回调，请根据相关错误码进行逻辑处理。

 @param delegate 托管对象
 @param roomId 房间ID
 @param userId 用户ID，必须是数字
  @param sessionId 会话ID，用于标识业务会话请求，每一次完整的流程之后，该值需要重新设置
 @param optionInfoDict 可选字典，若需旁路直播功能等可通过该字典设置
 例如:
 @{@"pull_addr":@"",//拉流地址
   @"push_addr":@"",//推流地址
  };
 @return 0 成功, 非0 表示失败。
 */
- (int) loadEngineDataWithDelegate:(nonnull id<QHVCInteractiveDelegate>)delegate
                            roomId:(nonnull NSString *)roomId
                            userId:(nonnull NSString *)userId
                         sessionId:(nullable NSString *)sessionId
                    optionInfoDict:(nullable NSDictionary *)optionInfoDict;


/**
 加入频道
 该方法让用户加入通话频道，在同一个频道内的用户可以互相通话，多个用户加入同一个频道，可以群聊。在真正加入频道后，SDK会触发didJoinChannel回调。如果已在通话中，用户必须调用leaveChannel退出当前通话，才能进入下一个频道。该方法是异步的，所以可以在主用户界面线程被调用。
 
 @return 0 成功, <0 表示失败.
 */
- (int)joinChannel;


/**
 离开频道
 即挂断或退出通话。joinChannel后，必须调用leaveChannel以结束通话，否则不能进行下一次通话。不管当前是否在通话中，都可以调用leaveChannel，没有副作用。leaveChannel会把会话相关的所有资源释放掉。leaveChannel是异步操作，调用返回时并没有真正退出频道。在真正退出频道后，SDK会触发didLeaveChannelWithStats回调。
 
 @return 0 成功, <0 表示失败.
 */
- (int)leaveChannel;


/**
 设置频道模式
 该方法用于设置频道模式(Profile)。需知道应用程序的使用场景(例如通信模式或直播模式),从而使用不同的优化手段。
 同一频道内只能同时设置一种模式。
 该方法必须在加入频道前调用和进行设置，进入频道后无法再设置。
 
 @param profile 频道模式
 @return 0 成功, return <0 表示失败
 */
- (int)setChannelProfile:(QHVCITLChannelProfile)profile;


/**
 开启视频模式
 该方法用于开启视频模式。可以在加入频道前或者通话中调用，在加入频道前调用，则自动开启视频模式，在通话中调用则由音频模式切换为视频模式。使用disableVideo方法可关闭视频模式。
 
 @return 0 成功, <0 表示失败.
 */
- (int)enableVideo;


/**
 关闭视频模式
 该方法用于关闭视频。可以在加入频道前或者通话中调用，在加入频道前调用，则自动开启纯音频模式，在通话中调用则由视频模式切换为纯音频频模式。使用enableVideo方法可开启视频模式。
 
 @return 0 成功, return <0 表示失败.
 */
- (int)disableVideo;


/**
 禁用本地视频功能
 禁用/启用本地视频功能。该方法用于只看不发的视频场景。该方法不需要本地有摄像头。
 
 @param enabled YES: 启用本地视频,NO: 禁用本地视频
 @return 0 成功, return <0 表示失败.
 */
- (int)enableLocalVideo:(BOOL)enabled;


/**
 该方法允许用户设置视频的优化选项。
 
 @param preferFrameRateOverImageQuality true: 画质和流畅度里，优先保证画质(默认),false: 画质和流畅度里，优先保证流畅度
 @return 0 成功, return <0 表示失败
 */
- (int)setVideoQualityParameters:(BOOL)preferFrameRateOverImageQuality;


/**
 开启视频预览
 该方法用于启动本地视频预览。在开启预览前，必须先调用setupLocalVideo设置预览窗口及属性，且必须调用enableVideo开启视频功能。如果在调用joinChannel进入频道之前调用了startPreview启动本地视频预览，在调用leaveChannel退出频道之后本地预览仍然处于启动状态，如需要关闭本地预览，需要额外调用stopPreview一次。
 
 @return 0 成功, return <0 表示失败
 */
- (int)startPreview;


/**
 停止视频预览
 该方法用于停止本地视频预览。
 
 @return 0 成功, return <0 表示失败
 */
- (int)stopPreview;


/**
 开启语音模式
 该方法开启语音模式(默认为开启状态)。
 
 @return 0 成功, return <0 表示失败
 */
- (int)enableAudio;


/**
 关闭语音模式
 
 @return 0 成功, return <0 表示失败
 */
- (int)disableAudio;


/**
 设置一些SDK的特殊参数
 
 @param options 必须是json格式
 @return 0 成功, return <0 表示失败
 */
- (int)setParameters:(nonnull NSString *)options;


/**
 设置用户角色
 在加入频道前，用户需要通过本方法设置观众(默认)或主播模式。在加入频道后，用户可以通过本方法切换用户模式。
 
 @param role 直播场景里的用户角色
 @param permissionKey 填写为NULL
 @return 0 成功, return <0 表示失败
 */
- (int)setClientRole:(QHVCITLClientRole)role withKey: (nullable NSString *)permissionKey;


/**
 获取连麦流的快照，数据返回通过takeStreamSnapshot返回
 
 @param uid 用户ID
 @return 0: 方法调用成功，<0: 方法调用失败
 */
- (int)takeStreamSnapshot:(nonnull NSString *)uid;


/**
 是否启用软件回声消除
 
 @param enable 是否开启软件回声消除
 @return 0: 方法调用成功，<0: 方法调用失败
 */
- (int)enableSoftwareAEC:(BOOL)enable;


/**
 启用测试环境

 @param testEnv YES：启用；NO：关闭
 @return 0：成功，<0:失败
 */
- (int)enableTestEnvironment:(BOOL)testEnv;


#pragma mark - 定义音频处理方法

/**
 启动音频外部渲染，一定要在loadEngineDataWithDelegate之前调用
 
 @param externalAudioRender TURE:启动外部音频处理，FALSE:不启动外部音频处理,默认:FALSE
 */
- (void) setExtenralAudioRender:(BOOL)externalAudioRender;


/**
 开启扬声器
 该方法切换音频输出方式：扬声器或听筒。在调用本方法前，请确保已成功调用joinChannel。
 
 @param enableSpeaker Yes： 音频输出至扬声器 No: 音频输出至听筒
 @return 0 成功, return <0 表示失败
 */
- (int)setEnableSpeakerphone:(BOOL)enableSpeaker;


/**
 是否是扬声器状态
 该方法检查扬声器是否已开启
 
 @return Yes:表明输出到扬声器 No：表明输出到非扬声器(听筒，耳机等)
 */
- (BOOL)isSpeakerphoneEnabled;


/**
 设置默认的语音路径
 该方法设置接收到的语音从听筒或扬声器出声。如果用户不调用本方法，语音默认从听筒出声。
 
 @param defaultToSpeaker YES: 从扬声器出声 NO: 从听筒出声
 @return 0 成功, return <0 表示失败
 */
- (int)setDefaultAudioRouteToSpeakerphone:(BOOL)defaultToSpeaker;


/**
 启用说话者音量提示
 该方法允许SDK定期向应用程序反馈当前谁在说话以及说话者的音量。
 
 @param interval 指定音量提示的时间间隔。<=0 ：禁用音量提示功能，>0：提示间隔，单位为毫秒。建议设置到大于200毫秒。
 @param smooth 平滑系数。默认可以设置为3。
 @return 0 成功, return <0 表示失败
 */
- (int)enableAudioVolumeIndication:(NSInteger)interval
                            smooth:(NSInteger)smooth;


/**
 将自己静音
 静音/取消静音。该方法用于允许/禁止往网络发送本地音频流。
 
 @param mute True：麦克风静音，False：取消静音
 @return 0 成功, return <0 表示失败
 */
- (int)muteLocalAudioStream:(BOOL)mute;


/**
 静音所有远端音频
 该方法用于允许/禁止播放远端用户的音频流，即对所有远端用户进行静音与否。
 
 @param mute True：停止播放所接收的音频流，False：恢复播放所接收的音频流
 @return 0 成功, return <0 表示失败
 */
- (int)muteAllRemoteAudioStreams:(BOOL)mute;


/**
 静音指定用户音频
 静音指定远端用户/对指定远端用户取消静音。本方法用于允许/禁止播放远端用户的音频流。
 
 @param uid 指定用户
 @param mute True：停止播放指定用户的音频流，False：恢复播放指定用户的音频流
 @return 0 成功, return <0 表示失败
 */
- (int)muteRemoteAudioStream:(nonnull NSString *)uid
                        mute:(BOOL)mute;


#pragma mark - 定义视频处理方法

/**
 启动视频外部渲染，一定要在loadEngineDataWithDelegate之前调用
 
 @param externalVideoRender TURE:打开外部图像渲染，FALSE:SDK渲染,默认:FALSE
 */
- (void) setExtenralVideoRender:(BOOL)externalVideoRender;


/**
 设置本地视频显示属性
 该方法设置本地视频显示信息。应用程序通过调用此接口绑定本地视频流的显示视窗(view)，并设置视频显示模式。
 在应用程序开发中， 通常在初始化后调用该方法进行本地视频设置，然后再加入频道。退出频道后，绑定仍然有效，如果需要解除绑定，可以指定空(NULL)View调用setupLocalVideo。
 
 @param local 设置视频属性
 @return 0 成功, return <0 表示失败
 */
- (int)setupLocalVideo:(nullable QHVCITLVideoCanvas *)local;


/**
 设置远端视频显示视图
 该方法绑定远程用户和显示视图，即设定uid指定的用户用哪个视图显示。调用该接口时需要指定远程视频的uid，一般可以在进频道前提前设置好，如果应用程序不能事先知道对方的uid，可以在应用程序收到didJoinedOfUid事件时设置。
 如果启用了视频录制功能，视频录制服务会做为一个哑客户端加入频道，因此其他客户端也会收到它的didJoinedOfUid事件，APP不应给它绑定视图（因为它不会发送视频流），如果APP不能识别哑客户端，可以在firstRemoteVideoDecodedOfUid事件触发时再绑定视图。解除某个用户的绑定视图可以把view设置为空。退出频道后，SDK会把远程用户的绑定关系清除掉。
 
 @param remote 设置视频属性
 @return 0 成功, return <0 表示失败
 */
- (int)setupRemoteVideo:(nonnull QHVCITLVideoCanvas *)remote;


/**
 移除远端观看视频
 
 @param remote 待移除的视频
 @return 0 成功, return <0 表示失败
 */
- (int)removeRemoteVideo:(nonnull QHVCITLVideoCanvas *)remote;


/**
 设置本地视频显示模式，该方法设置本地视频显示模式。应用程序可以多次调用此方法更改显示模式。
 
 @param mode 视频显示模式
 @return 0 成功, return <0 表示失败
 */
- (int)setLocalRenderMode:(QHVCITLRenderMode) mode;


/**
 设置远端视频显示模式
 该方法设置远端视频显示模式。应用程序可以多次调用此方法更改显示模式。
 
 @param uid 用户ID，指定远程视频来自哪个用户。
 @param mode 视频显示模式
 @return 0 成功, return <0 表示失败
 */
- (int)setRemoteRenderMode:(nonnull NSString *)uid
                      mode:(QHVCITLRenderMode)mode;


/**
 切换前置/后置摄像头
 该方法用于在前置/后置摄像头间切换。
 
 @return 0 成功, return <0 表示失败
 */
- (int)switchCamera;


/**
 设置本地视频属性
 该方法设置视频编码属性。
 每个Profile对应一套视频参数，如分辨率、帧率、码率等。当设备的摄像头不支持指定的分辨率时，SDK会自动选择一个合适的摄像头分辨率，但是编码分辨率仍然用setVideoProfile 指定的。
 该方法仅设置编码器编出的码流属性，可能跟最终显示的属性不一致，例如编码码流分辨率为640x480，码流的旋转属性为90度，则显示出来的分辨率为竖屏模式。
 应在调用joinChannel/startPreview前设置视频属性。
 
 @param profile 视频属性
 @param swapWidthAndHeight 是否交换宽和高，true：交换宽和高，false：不交换宽和高(默认)
 @return 0 成功, return <0 表示失败
 */
- (int)setVideoProfile:(QHVCITLVideoProfile)profile
    swapWidthAndHeight:(BOOL)swapWidthAndHeight;


/**
 开启、关闭双流模式
 
 @param enabled true开启双流模式，FALSE使用单流模式
 @return 0 成功, return <0 表示失败
 */
- (int)enableDualStreamMode:(BOOL)enabled;


/**
 设置小流视频参数，此方法只有在enableDualStreamMode开启时才生效，而且要在joinChannel之前调用。
 
 @param width 视频宽
 @param height 视频高
 @param fps 帧率
 @param bitrate 码率
 @return 0 成功，非0失败
 */
- (int)setLowStreamVideoProfile:(int)width
                         height:(int)height
                            fps:(int)fps
                        bitrate:(int)bitrate;


/**
 设置视频大小流
 该方法指定接收远端用户的视频流大小。使用该方法可以根据视频窗口的大小动态调整对应视频流的大小，以节约带宽和计算资源。
 SDK默认收到视频小流，节省带宽。如需使用视频大流，调用本方法进行切换。
 
 @param uid 用户ID
 @param streamType 设置视频流大小
 @return 0 成功, return <0 表示失败
 */
- (int)setRemoteVideoStream: (nonnull NSString *) uid
                       type: (QHVCITLVideoStreamType) streamType;


/**
 暂停发送本地视频流
 暂停/恢复发送本地视频流。该方法用于允许/禁止往网络发送本地视频流。
 
 @param mute Yes: 暂停发送本地视频流，No: 恢复发送本地视频流
 @return 0 成功, return <0 表示失败
 */
- (int)muteLocalVideoStream:(BOOL)mute;


/**
 暂停所有远端视频流
 本方法用于允许/禁止播放所有人的视频流。
 
 @param mute Yes: 停止播放接收到的所有视频流，No: 允许播放接收到的所有视频流
 @return 0 成功, return <0 表示失败
 */
- (int)muteAllRemoteVideoStreams:(BOOL)mute;


/**
 暂停指定远端视频流
 该方法用于允许/禁止播放指定的远端视频流。
 
 @param uid 用户ID
 @param mute Yes: 停止播放接收到的视频流，No: 允许播放接收到的视频流
 @return 0 成功, return <0 表示失败
 */
- (int)muteRemoteVideoStream:(nonnull NSString *)uid
                        mute:(BOOL)mute;


#pragma mark - 合流布局相关方法
/**
 n+m路转推（n路分流+m路合流），合流信息预设接口
 请确保要在joinChannel之前调用。
 
 @param mixerPublisherConfiguration 合流转推配置信息
 @return 0 成功, <0 表示失败.
 */
- (int)setMixStreamInfo:(nonnull QHVCITLMixerPublisherConfiguration *)mixerPublisherConfiguration ;


/**
 设置画中画布局
 该方法设置直播场景里的画中画布局。
 1、您需要首先定义一个画布(canvas): 画布的宽和高(即视频的分辨率), 背景颜色，和您想在屏幕上显示的视频总数。
 2、您需要在画布上定义每个视频的位置和尺寸(无论画布定义的宽和高有多大，每个视频用0到1的相对位置和尺寸进行定义)，图片所在的图层，图片的透明度，视频是经过裁减的还是缩放到合适大小等等。
 
 @param layout 布局参数
 @return 0 成功, return <0 表示失败
 */
- (int)setVideoCompositingLayout:(nonnull QHVCITLVideoCompositingLayout *)layout;


/**
 取消画中画布局设置
 当该合流任务取消时，需要调用此方法。
 
 @return 0: 方法调用成功，<0: 方法调用失败
 */
- (int)clearVideoCompositingLayout;


#pragma mark - 外部数据采集相关
/**
 开启数据采集
 */
- (void)openCollectingData;


/**
 关闭数据采集
 */
- (void)closeCollectingData;


/**
 采集数据
 
 @param buffer CVPixelBufferRef
 @param time 时间戳
 */
- (void)incomingCollectingCapturedData:(nonnull CVPixelBufferRef)buffer
                             timeStamp:(CMTime)time;


#pragma mark - 外部处理音视频方法设置


/**
 设置视频回调代理，此代理生效必须在此之前打开视频处理开关
 
 @param videoFrameDelegate 视频处理对象
 */
- (void)setVideoFrameDelegate:(nullable id<QHVCInteractiveVideoFrameDelegate>)videoFrameDelegate;


/**
 设置音频回调代理，此代理生效必须在此之前打开音频处理开关
 
 @param audioFrameDelegate 音频处理对象
 */
- (void)setAudioFrameDelegate:(nullable id<QHVCInteractiveAudioFrameDelegate>)audioFrameDelegate;

@end
