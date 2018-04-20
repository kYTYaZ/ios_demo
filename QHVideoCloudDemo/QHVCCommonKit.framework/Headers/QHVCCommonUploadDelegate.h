//
//  QHVCCommonUploadDelegate.h
//  QHVCCommonKit
//
//  Created by deng on 2018/4/4.
//  Copyright © 2018年 qihoo.QHVCCommonKit. All rights reserved.
//

#ifndef QHVCCommonUploadDelegate_h
#define QHVCCommonUploadDelegate_h

NS_ASSUME_NONNULL_BEGIN

@protocol QHVCUploaderDelegate <NSObject>

/**
 *  @功能 回调上传状态 成功、失败
 *  @参数 uploader 调用者创建的uploader
 *  @参数 status 上传状态
 */
- (void)didUpload:(id)uploader status:(QHVCUploadStatus)status error:(nullable NSError *)error;

@optional
/**
 *  @功能 上传进度
 *  @参数 uploader 调用者创建的uploader
 *  @参数 progress 上传进度（0.0-1.0）
 */
- (void)didUpload:(id)uploader progress:(float)progress;

@end

@protocol QHVCRecorderDelegate <NSObject>

/**
 *  @功能 保存持久化上传信息
 *  @参数 key 持久化记录key
 *  @参数 data 上传信息
 */
- (void)setRecorder:(NSString *)key data:(NSData *)data;

/**
 *  @功能 获取上传信息
 *  @参数 key 持久化记录key
 *  @返回值 存储的上传信息
 */
- (NSData *)fetchRecorder:(NSString *)key;

/**
 *  @功能 删除上传信息（上传成功、信息过期）
 *  @参数 key 持久化记录key
 */
- (void)deleteRecorder:(NSString *)key;

@end

NS_ASSUME_NONNULL_END

#endif /* QHVCCommonUploadDelegate_h */
