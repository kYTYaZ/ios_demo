//
//  QHVCEditStickerIconView.h
//  QHVideoCloudToolSet
//
//  Created by deng on 2018/1/15.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import <UIKit/UIKit.h>

@class QHVCEditStickerIconView;

typedef void(^DeleteAction)(QHVCEditStickerIconView *sticker);

@interface QHVCEditStickerIconView : UIView

@property (nonatomic, copy) DeleteAction deleteCompletion;
@property (nonatomic, strong) UIImageView *sticker;
@property (nonatomic, strong) UILabel *subtitle;
@property (nonatomic, assign) CGFloat rotateAngle;

- (void)setStickerImageUrl:(NSString *)imageUrl;

@end
