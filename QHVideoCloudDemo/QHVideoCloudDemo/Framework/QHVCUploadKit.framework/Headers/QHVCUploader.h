//
//  QHVCUploader.h
//  QHVCUploadKit
//
//  Created by deng on 2017/9/18.
//  Copyright © 2017年 qihoo. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, QHVCUploadTaskType) {
    QHVCUploadTaskTypeUnknow = 0,
    QHVCUploadTaskTypeForm,//表单
    QHVCUploadTaskTypeParallel//分片
};

typedef NS_ENUM(NSInteger, QHVCUploadStatus) {
    QHVCUploadStatusUploadSucceed = 3,
    QHVCUploadStatusUploadFail,
    QHVCUploadStatusUploadError,
    QHVCUploadStatusUploadCancel
};

typedef NS_ENUM(NSInteger, QHVCUploadError) {
    QHVCUploadErrorNull = 0,//
    QHVCUploadErrorResponse = 20,//上传返回值解析异常
    QHVCUploadErrorInvalidToken = -111,//Token为空
    QHVCUploadErrorInvalidData = -112,//上传的内存数据是空
    QHVCUploadErrorInvalidFile = -113,//上传的文件0字节
    QHVCUploadErrorFileIsDir = -108,//指定FILE是文件夹
    QHVCUploadErrorFileNoExist = -105,//文件不存在
};

typedef NS_ENUM(NSInteger, QHVCUploadLogLevel) {
    QHVCUploadLogLevelTrace = 0,
    QHVCUploadLogLevelDebug = 1,
    QHVCUploadLogLevelInfo  = 2,
    QHVCUploadLogLevelWarn  = 3,
    QHVCUploadLogLevelError = 4,
    QHVCUploadLogLevelAlarm = 5,
    QHVCUploadLogLevelFatal = 6,
};

NS_ASSUME_NONNULL_BEGIN

@class QHVCUploader;

@protocol QHVCUploaderDelegate <NSObject>

/**
 *  @功能 回调上传状态 成功、失败
 *  @参数 uploader
 *  @参数 status 上传状态
 */
- (void)didUpload:(QHVCUploader *)uploader status:(QHVCUploadStatus)status error:(nullable NSError *)error;

@optional
/**
 *  @功能 上传进度
 *  @参数 uploader
 *  @参数 progress 上传进度（0.0-1.0）
 */
- (void)didUpload:(QHVCUploader *)uploader progress:(float)progress;

@end

@protocol QHVCRecorderDelegate <NSObject>

@end

@interface QHVCUploader : NSObject

/**
 *  @功能 获取上传类型，目前有表单和分片两种形式，具体使用哪种形式由服务器返回的配置信息决定
 *  如果是分片上传，需要调用parallelQueueNum获得队列数，用于计算token
 *  如果是表单上传，无需调用parallelQueueNum，计算token不需要此参数
 *  @参数 size 待上传任务数据大小，单位：字节
 *  @返回值
 */
- (QHVCUploadTaskType)uploadTaskType:(uint64_t)size;

/**
 *  @功能 获取分片上传队列数，用于业务计算token
 *  @返回值 分片上传队列数
 */
- (NSInteger)parallelQueueNum;

/**
 *  @功能 两种上传方式，数据在本地uploadFile:，数据在内存中uploadData:
 *  @参数 filePath 待上传文件本地路径
 *  @参数 data    待上传内存数据
 *  @参数 fileName    本地文件/内存数据上传到服务器后的文件名
 *  @参数 token 表单/分片任务计算规则略有差别 在服务器计算
 */
- (void)uploadFile:(NSString *)filePath fileName:(NSString *)fileName token:(NSString *)token;
- (void)uploadData:(NSData *)data fileName:(NSString *)fileName token:(NSString *)token;

/**
 *  @功能 取消当前上传任务
 */
- (void)cancel;

/**
 *  @功能 上传直播云本地日志到服务器，回调流程同正常上传
 *  若用户遇到问题，业务方可通过该接口将用户的本地日志文件上传到直播云日志服务器
 */
- (void)uploadLogs;

/**
 *  @功能 设置回调代理
 *  @参数
 */
- (void)setUploaderDelegate:(nullable id<QHVCUploaderDelegate>)uploaderDelegate;
- (nullable id<QHVCUploaderDelegate>)uploaderDelegate;

#pragma mark Common

/**
 *  @功能 第三方设置上传域名，上传前设置
 *  @参数 domain 有效的域名
 * （北京上传地址：up-beijing.oss.yunpan.360.cn
 *  上海上传地址：up-shanghai.oss.yunpan.360.cn）
 */
+ (void)setUploadDomain:(NSString *)domain;

//统计相关，请正确设置，利于排查线上问题，在上传前设置

/**
 *  @功能 设置统计信息
 *  @参数 info 
 @{@"businessId":@"",
 @"channelId":@"",
 @"userId":@"",
 @"deviceId":@"",
 @"appVersion":@""
 };
 */
+ (void)setStatisticsInfo:(NSDictionary *)info;

/**
 *  @功能 用户id    setStatisticsInfo代替
 *  @参数 userId  第三方用户id
 */
+ (void)setUserId:(NSString *)userId DEPRECATED_ATTRIBUTE;

/**
 *  @功能 设置第三方渠道号    setStatisticsInfo代替
 *  @参数 channelId   渠道号
 */

+ (void)setChannelId:(NSString *)channelId DEPRECATED_ATTRIBUTE;
/**
 *  @功能 设置第三方业务版本号  setStatisticsInfo代替
 *  @参数 appVersion   版本号
 */
+ (void)setAppVersion:(NSString *)appVersion DEPRECATED_ATTRIBUTE;

/**
 *  @功能 设置设备id  setStatisticsInfo代替
 *  @参数 deviceId   设备id
 */
+ (void)setDeviceId:(NSString *)deviceId DEPRECATED_ATTRIBUTE;

/**
 * 开启日志（debug阶段辅助开发调试，根据实际情况使用）
 * @参数 level 日志等级
 */
+ (void)openLogWithLevel:(QHVCUploadLogLevel)level;

/**
 * 设置日志输出callback
 * @参数 callback 回调block
 */
+ (void)setLogOutputCallBack:(void(^)(int loggerID, QHVCUploadLogLevel level, const char *data))callback;

/**
 *  @功能 获取上传sdk版本号
 *  @返回值 sdk版本号（e.g. 2.0.0）
 */
+ (NSString *)sdkVersion;

@end

NS_ASSUME_NONNULL_END
