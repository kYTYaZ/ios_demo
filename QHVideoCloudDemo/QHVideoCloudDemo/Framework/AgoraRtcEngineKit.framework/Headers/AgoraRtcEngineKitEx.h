//
//  AgoraRtcEngineKit.h
//  AgoraRtcEngineKit
//
//  Created by Sting Feng on 2014-8-11.
//  Copyright (c) 2015 Agora. All rights reserved.
//
//  v1.1
#include "AgoraRtcEngineKit.h"

@protocol AgoraRtcEngineExtensionDelegate <AgoraRtcEngineDelegate>
@optional
- (void)rtcEngine:(AgoraRtcEngineKit *)engine recapData:(const char *)recapData length:(NSInteger)length;
- (void)rtcEngine:(AgoraRtcEngineKit *)engine audioTransportQualityOfUid:(NSUInteger)uid delay:(NSUInteger)delay lost:(NSUInteger)lost;
- (void)rtcEngine:(AgoraRtcEngineKit *)engine videoTransportQualityOfUid:(NSUInteger)uid delay:(NSUInteger)delay lost:(NSUInteger)lost;
@end


@interface AgoraRtcEngineKit (AgoraExtension)

+ (instancetype)sharedEngineWithAppId:(NSString*)appId
                    extensionDelegate:(id<AgoraRtcEngineExtensionDelegate>)delegate;

/**
 *  Specify sdk profile
 *
 *  @param profile sdk profile in json format.
 *  @param merge specify merge or replace old profile
 */
- (int)setProfile:(NSString *)profile
            merge:(BOOL)merge;

/** END OF COMMON METHODS */

/** BEGIN OF AUDIO METHODS */


/**
 *  Enable recap
 *
 *  @param interval <=0 - disabled, >0 interval in ms.
 */
- (int)enableRecap:(NSInteger)interval;

/**
 *  Start playing recap conversation
 *
 */
- (int)playRecap;

- (int)enableAudioQualityIndication:(BOOL)enabled;
- (int)enableTransportQualityIndication:(BOOL)enabled;

- (int)setVideoProfileEx: (NSInteger)width andHeight: (NSInteger)height andFrameRate: (NSInteger)frameRate andBitrate: (NSInteger) andBitrate;

- (int)sendReportData:(NSData*) data
                 type:(NSInteger)type;
/** END OF AUDIO METHODS */

/**
 * query internal states
 * @param parameters
 *     json string, array type
 * @return a json string
 */
- (NSString *)getParameters:(NSString *)parameters;

/**
 *  Generates a URL linking to the call quality reports.
 *
 *  @param channel      The channel name specified in the joinChannel method.
 *  @param listenerUid  The uid of the listener.
 *  @param speakerUid   The uid of the speaker.
 *  @param reportFormat The format of the report.
                        AgoraRtc_QualityReportFormat_Json (0): JSON.: Returns the quality report data in Json.
                        AgoraRtc_QualityReportFormat_Html (1): HTML.: Returns a report in HTML format, displayed on a web browser or WebVIEW components.
 *
 *  @return 0 when executed successfully. return minus value when failed. return AgoraRtc_Error_Invalid_Argument (-2)：Invalid argument. return AgoraRtc_Error_Buffer_Too_Small (-6)：The buffer length is too small.
 */
- (NSString*) makeQualityReportUrl:(NSString*) channel
                       listenerUid:(NSUInteger) listenerUid
                       speakerrUid:(NSUInteger) speakerUid
                      reportFormat:(AgoraRtcQualityReportFormat) reportFormat;

/*********************************************************
 * Large group conference call (experiment) - END
 *********************************************************/
@end
