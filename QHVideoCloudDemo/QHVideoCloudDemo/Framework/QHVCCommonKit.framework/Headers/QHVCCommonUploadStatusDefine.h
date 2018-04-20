//
//  QHVCCommonUploadStatusDefine.h
//  QHVCCommonKit
//
//  Created by deng on 2018/4/4.
//  Copyright © 2018年 qihoo.QHVCCommonKit. All rights reserved.
//

#ifndef QHVCCommonUploadStatusDefine_h
#define QHVCCommonUploadStatusDefine_h

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

#endif /* QHVCCommonUploadStatusDefine_h */
