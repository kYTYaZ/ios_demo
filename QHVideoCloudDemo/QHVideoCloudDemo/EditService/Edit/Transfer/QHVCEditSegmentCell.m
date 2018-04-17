//
//  QHVCEditSegmentCell.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2018/2/5.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import "QHVCEditSegmentCell.h"
#import "QHVCEditPrefs.h"
#import "QHVCEditSegmentItem.h"

#define kTransferStyle @"edit_transfer_style_"

@interface QHVCEditSegmentCell()
{
    __weak IBOutlet UIImageView *_thumbImageView;
    __weak IBOutlet UILabel *_beginLabel;
    __weak IBOutlet UILabel *_endLabel;
    __weak IBOutlet UIButton *_transferBtn;
}
@property (nonatomic, strong) QHVCEditSegmentItem *segmentItem;

@end

@implementation QHVCEditSegmentCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)updateCell:(QHVCEditSegmentItem *)item isHighlight:(BOOL)isHighlight
{
    self.segmentItem = item;
    
    _thumbImageView.image = item.thumbnail;
    _beginLabel.text = [QHVCEditPrefs timeFormatMs:item.segmentStartTime];
    _endLabel.text = item.transferType == QHVCEditTransferTypeAdd?[QHVCEditPrefs timeFormatMs:item.segmentEndTime]:@"";
    
    if (item.transferType == QHVCEditTransferTypeAdd) {
        [_transferBtn setImage:[UIImage imageNamed:@"edit_transfer_add"] forState:UIControlStateNormal];
    }
    else
    {
        NSString *imageName = nil;
        if (isHighlight) {
            imageName = [NSString stringWithFormat:@"%@%@_h",kTransferStyle,@(item.transferType)];
        }
        else
        {
            imageName = [NSString stringWithFormat:@"%@%@",kTransferStyle,@(item.transferType)];
        }
        [_transferBtn setImage:[UIImage imageNamed:imageName] forState:UIControlStateNormal];
    }
}

- (IBAction)transferBtnAction:(UIButton *)sender
{
    if (self.selectedCompletion) {
        self.selectedCompletion(self.segmentItem);
    }
}

@end
