//
//  QHVCEditQualityView.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2018/1/5.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import "QHVCEditQualityView.h"
#import "QHVCEditPrefs.h"

@interface QHVCEditQualityView()
{
    __weak IBOutlet UIView *_containerView;
}
@end

@implementation QHVCEditQualityView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    NSInteger i = 0;
    for (NSNumber *v in [QHVCEditPrefs sharedPrefs].qualitys) {
        UIView *view = [_containerView viewWithTag:i+1];
        UISlider *slider = [view viewWithTag:i];
        slider.value = v.floatValue;
        i++;
    }
}

- (IBAction)slider:(UISlider *)sender
{
    if (self.changeCompletion) {
        self.changeCompletion(sender.tag, sender.value);
    }
}

@end
