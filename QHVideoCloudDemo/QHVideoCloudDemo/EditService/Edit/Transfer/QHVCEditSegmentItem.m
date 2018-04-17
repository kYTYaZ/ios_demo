//
//  QHVCEditSegmentItem.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2018/2/5.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import "QHVCEditSegmentItem.h"

@implementation QHVCEditSegmentItem

- (instancetype)init
{
    self = [super init];
    if (self) {
        _transferType = QHVCEditTransferTypeNone;
    }
    return self;
}

@end
