//
//  QHVCEdit.h
//  QHVCEditKit
//
//  Created by liyue-g on 2017/9/11.
//  Copyright © 2017年 liyue-g. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class QHVCEditCommandFactory;

typedef NS_ENUM(NSUInteger, QHVCEditPlayerError)
{
    QHVCEditPlayerError_NoError       = 1, //无错误
    QHVCEditPlayerError_ParamError    = 2, //参数错误
    QHVCEditPlayerError_InitError     = 3, //初始化失败
    QHVCEditPlayerError_StatusError   = 4, //播放状态错误
};

typedef NS_ENUM(NSUInteger, QHVCEditPlayerErrorInfo)
{
    QHVCEditPlayerErrorInfo_NoError             = 1, //无错误
    QHVCEditPlayerErrorInfo_CommandFactoryError = 2, //edit handle错误
    QHVCEditPlayerErrorInfo_PlayerHandleError   = 3, //播放器handle错误
};

typedef NS_ENUM(NSUInteger, QHVCEditPlayerPreviewFillMode)
{
    QHVCEditPlayerPreviewFillMode_AspectFit,    //视频内容完全填充，可能会有黑边
    QHVCEditPlayerPreviewFillMode_AspectFill,   //视频内容铺满画布，视频内容可能会被裁剪
    QHVCEditPlayerPreviewFillMode_ScaleToFill,  //视频内容铺满画布，视频内容可能会被拉伸
};

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

@interface QHVCEditPlayer : NSObject


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

@end





