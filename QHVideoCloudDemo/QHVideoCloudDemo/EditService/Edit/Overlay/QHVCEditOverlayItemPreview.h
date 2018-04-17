//
//  QHVCEditOverlayItemPreview.h
//  QHVideoCloudToolSet
//
//  Created by liyue-g on 2018/2/28.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void(^TapAction)();
typedef void (^PlayerNeedRefreshAction)();

@interface QHVCEditOverlayItemPreview : UIView

- (void)setSelect:(BOOL)select;

@property (nonatomic, readwrite, retain) UIView* overlay;
@property (nonatomic, readwrite, copy)   TapAction tapAction;
@property (nonatomic, readwrite, copy)   PlayerNeedRefreshAction playerNeedRefreshAction;
@property (nonatomic, readwrite, assign) CGFloat radian;
@property (nonatomic, readonly,  assign) BOOL isCroping;
@property (nonatomic, readonly,  assign) CGRect cropRect;

@property (nonatomic, readwrite, assign) NSInteger overlayCommandId;
@property (nonatomic, readwrite, assign) NSInteger startTimestampMs;
@property (nonatomic, readwrite, assign) NSInteger endTimestampMs;

- (void)startCrop;
- (void)stopCrop:(BOOL)confirm;

- (void)showColorPicker;
- (void)hideColorPicker;
- (void)updateChromakey:(int)threshlod extend:(int)extend;

@end
