//
//  QHVCITSLinkMicViewController.h
//  QHVideoCloudToolSet
//
//  Created by yangkui on 2018/3/15.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QHVCITSDefine.h"
#import "QHVCITSConfig.h"
#import "QHVCITSRoomModel.h"

@interface QHVCITSLinkMicViewController : UIViewController
{
    IBOutlet UICollectionView *_actionCollectionView;
    IBOutlet UIButton *_existFullScreenBtn;
    IBOutlet UIView *_controlView;
    
    NSMutableArray<NSDictionary *> *_actionsArray;
    
    BOOL _openSpeakerphone;
    BOOL _openFrontCamera;
    BOOL _muteMicrophone;
    BOOL _muteLocalVideo;
    BOOL _muteAllRemoteVideo;
    
    BOOL _isFullScreen;
}

@end
