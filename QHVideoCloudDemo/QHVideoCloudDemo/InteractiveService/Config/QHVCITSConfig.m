//
//  QHVCITSConfig.m
//  QHVideoCloudToolSet
//
//  Created by yangkui on 2018/3/15.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import "QHVCITSConfig.h"

@implementation QHVCITSConfig

+ (nonnull NSString *)getVersion
{
    return @"1.0.0.0";
}

+ (nonnull instancetype)sharedInstance
{
    static QHVCITSConfig * s_instance = NULL;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        s_instance = [QHVCITSConfig new];
    });
    return s_instance;
}

- (id)init
{
    self = [super init];
    [self setInteractiveServerUrl:QHVCITS_RELEASE_ENV_INTERACTIVE_SERVER_URL];
    _protocolQueue = dispatch_queue_create("its.protocol.process.queue", NULL);
    dispatch_queue_set_specific(_protocolQueue, _protocolQueueKey, (__bridge void *)self, NULL);
    _userHeartInterval = QHVCITS_HEART_TIME_INTERTVAL;
    _updateRoomListInterval = QHVCITS_ROOMLIST_TIME_INTERTVAL;
    _bypassLiveCDNUseSDKService = NO;
    _dataCollectMode = QHVCITLDataCollectModeSDK;
    _businessRenderMode = QHVCITS_Render_SDK;
    _videoEncoderProfile = QHVCITL_VideoProfile_360P_9;
    _videoEncoderProfileForGuest = QHVCITL_VideoProfile_360P_9;
    _videoViewRenderMode = QHVCITL_Render_ScaleAspectFill;
    _mergeVideoCanvasWidth = QHVCITS_VIDEO_WIDTH;
    _mergeVideoCanvasHeight = QHVCITS_VIDEO_HEIGHT;
    
    NSString* path = [[NSBundle mainBundle] pathForResource:@"InteractiveVideoProfile" ofType:@"plist"];
    _videoProfiles = [NSMutableArray arrayWithContentsOfFile:path];
    
    return self;
}

- (void) setEnableTestEnvironment:(BOOL)enableTestEnvironment
{
    if (enableTestEnvironment)
    {
        [self setInteractiveServerUrl:QHVCITS_TEST_ENV_INTERACTIVE_SERVER_URL];
    }else
    {
        [self setInteractiveServerUrl:QHVCITS_RELEASE_ENV_INTERACTIVE_SERVER_URL];
    }
    _enableTestEnvironment = enableTestEnvironment;
}

@end
