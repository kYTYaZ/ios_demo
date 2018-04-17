//
//  QHVCEditSubtitleViewController.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2018/1/19.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import "QHVCEditSubtitleViewController.h"
#import "QHVCEditSubtitleView.h"
#import "QHVCEditPrefs.h"
#import "QHVCEditFrameView.h"
#import "QHVCEditSubtitleItem.h"
#import "QHVCEditCommandManager.h"
#import "QHVCEditStickerIconView.h"

@interface QHVCEditSubtitleViewController ()
{
    QHVCEditSubtitleView *_subtitleView;
    QHVCEditFrameView *_frameView;
    QHVCEditFrameStatus _viewType;
    QHVCEditSubtitleItem *_currentSubtitleItem;
    QHVCEditStickerIconView *_sticker;
    NSArray *_colorsArray;
    BOOL _hasChange;
}
@property (nonatomic, strong) NSMutableArray<NSMutableArray *> *subtitleInfos;

@end

@implementation QHVCEditSubtitleViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.titleLabel.text = @"字幕";
    [self.nextBtn setTitle:@"确定" forState:UIControlStateNormal];
    
    _hasChange = NO;
    _subtitleInfos = [NSMutableArray arrayWithArray:[QHVCEditPrefs sharedPrefs].subtitleTimestamp];
    
    _colorsArray = @[@[@"edit_color_black",@"000000"],@[@"edit_color_blue",@"125FDF"],
                     @[@"edit_color_gray",@"888888"],@[@"edit_color_green",@"25B727"],@[@"edit_color_pink",@"FE8AB1"],
                     @[@"edit_color_red",@"F54343"],@[@"edit_color_white",@"FFFFFF"],@[@"edit_color_yellow",@"FFDB4F"]];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onKeyboardNotification:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onKeyboardNotification:) name:UIKeyboardWillHideNotification object:nil];
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self createFrameView];
    _sliderViewBottom.constant = 70;
}

- (void)createFrameView
{
    _frameView = [[NSBundle mainBundle] loadNibNamed:[[QHVCEditFrameView class] description] owner:self options:nil][0];
    _frameView.frame = CGRectMake(0, kScreenHeight - 160, kScreenWidth, 160);
    _frameView.duration = self.durationMs;
    _frameView.timeStamp = [QHVCEditPrefs sharedPrefs].subtitleTimestamp;
    _viewType = QHVCEditFrameStatusAdd;
    [_frameView setUIStatus:_viewType];
    
    __weak typeof(self) weakSelf = self;
    _frameView.addCompletion = ^(NSTimeInterval insertStartMs) {
        [weakSelf handleAddAction:insertStartMs];
    };
    _frameView.doneCompletion = ^(NSTimeInterval insertEndMs) {
        [weakSelf handleDoneAction:insertEndMs];
    };
    _frameView.editCompletion = ^{
        [weakSelf handleEditAction];
    };
    _frameView.discardCompletion = ^{
        [weakSelf handleDiscardAction];
    };
    [self.view addSubview:_frameView];
}

- (void)handleAddAction:(NSTimeInterval)insertStartMs
{
    [self updateViewType:QHVCEditFrameStatusEdit];
    
    _currentSubtitleItem = [[QHVCEditSubtitleItem alloc] init];
    _currentSubtitleItem.insertStartTimeMs = insertStartMs;
    
    _subtitleView = [[NSBundle mainBundle] loadNibNamed:[[QHVCEditSubtitleView class] description] owner:self options:nil][0];
    _subtitleView.frame = CGRectMake(0, kScreenHeight - 170, kScreenWidth, 170);
    _subtitleView.subtitleItem = _currentSubtitleItem;
    _subtitleView.colorsArray = _colorsArray;
    __weak typeof(self) weakSelf = self;
    _subtitleView.refreshCompletion = ^(QHVCEditSubtitleItem *item) {
        [weakSelf handleRefreshSticker:item];
    };
    [self.view addSubview:_subtitleView];
}

- (void)handleRefreshSticker:(QHVCEditSubtitleItem *)item
{
    if (_sticker) {
        [self updateSticker:item];
    }
    else
    {
        [self addSticker:item];
    }
}

- (void)updateSticker:(QHVCEditSubtitleItem *)item
{
    _sticker.sticker.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@_%@",kStylesName,@(item.styleIndex)]];
    [self updateSubtitleText:item];
}

- (void)updateSubtitleText:(QHVCEditSubtitleItem *)item
{
    _sticker.subtitle.text = item.subtitleText;
    _sticker.subtitle.font = [UIFont systemFontOfSize:item.fontValue];
    _sticker.subtitle.textColor = [QHVCEditPrefs colorHex:_colorsArray[item.colorIndex][1]];
}

- (void)addSticker:(QHVCEditSubtitleItem *)item
{
    CGFloat x = [QHVCEditPrefs randomNum:0 to:_preview.width - 100];
    CGFloat y = [QHVCEditPrefs randomNum:0 to:_preview.height - 100];
    
    _sticker = [[QHVCEditStickerIconView alloc] initWithFrame:CGRectMake(x, y, 100, 100)];
    [self updateSticker:item];
    
    __weak typeof(self) weakSelf = self;
    _sticker.deleteCompletion = ^(QHVCEditStickerIconView *sticker) {
        [weakSelf handleDeleteAction];
    };
    [_preview addSubview:_sticker];
}

- (void)handleDeleteAction
{
    [_subtitleView resetView];
}

- (void)handleDoneAction:(NSTimeInterval)insertEndMs
{
    [self nextAction:nil];
}

- (void)handleEditAction
{
    [self backAction:nil];
}

- (void)handleDiscardAction
{
    [_subtitleView removeFromSuperview];
    _subtitleView = nil;
    
    [self updateViewType:QHVCEditFrameStatusAdd];
}

- (void)updateViewType:(QHVCEditFrameStatus)status
{
    _viewType = status;
    
    if (status == QHVCEditFrameStatusAdd) {
        self.titleLabel.text = @"字幕";
        [_frameView setUIStatus:_viewType];
        _frameView.hidden = NO;
        _sliderViewBottom.constant = 70;
        
        [_frameView removeUncompleteTimestamp];
    }
    else if (status == QHVCEditFrameStatusEdit)
    {
        self.titleLabel.text = @"选择";
        _frameView.hidden = YES;
        _sliderViewBottom.constant = 70;
    }
    else if (status == QHVCEditFrameStatusDone)
    {
        self.titleLabel.text = @"添加";
        [_frameView setUIStatus:_viewType];
        _frameView.hidden = NO;
        _sliderViewBottom.constant = 70;
    }
}

- (void)nextAction:(UIButton *)btn
{
    if(_viewType == QHVCEditFrameStatusEdit)
    {
        if (_currentSubtitleItem.styleIndex >= 0) {
            _subtitleView.hidden = YES;

            [self updateViewType:QHVCEditFrameStatusDone];
        }
        else
        {
            [_subtitleView removeFromSuperview];
            _subtitleView = nil;

            [self updateViewType:QHVCEditFrameStatusAdd];
        }
    }
    else if (_viewType == QHVCEditFrameStatusDone)
    {
        [_subtitleView removeFromSuperview];
        _subtitleView = nil;

        [self updateViewType:QHVCEditFrameStatusAdd];

        UIImage *composeImage = [QHVCEditPrefs convertViewToImage:_sticker.sticker];
        _currentSubtitleItem.composeImage = composeImage;
        _currentSubtitleItem.insertEndTimeMs = [_frameView fetchCurrentTimeStampMs];
        
        [[QHVCEditCommandManager manager] addSubtitles:@[_currentSubtitleItem] views:@[_sticker]];
        
        [_sticker removeFromSuperview];
        _sticker = nil;
        
        [self resetPlayer:0.0];
        _hasChange = YES;
    }
    else
    {
        if (_hasChange) {
            if (self.confirmCompletion) {
                self.confirmCompletion(QHVCEditPlayerStatusReset);
            }
            [[QHVCEditCommandManager manager] updateSubtitles];
        }
        [self releasePlayerVC];
    }
}

- (void)backAction:(UIButton *)btn
{
    if(_viewType == QHVCEditFrameStatusEdit)
    {
        [_subtitleView removeFromSuperview];
        _subtitleView = nil;
        
        if (_sticker) {
            [_sticker removeFromSuperview];
            _sticker = nil;
        }
        [self updateViewType:QHVCEditFrameStatusAdd];
    }
    else if (_viewType == QHVCEditFrameStatusDone)
    {
        _subtitleView.hidden = NO;

        [self updateViewType:QHVCEditFrameStatusEdit];
    }
    else
    {
        if (_hasChange) {
            [[QHVCEditCommandManager manager] deleteSubtitles];
        }
        [QHVCEditPrefs sharedPrefs].subtitleTimestamp = _subtitleInfos;
        [self releasePlayerVC];
    }
}

- (void)onKeyboardNotification:(NSNotification *)notif
{
    NSDictionary *userInfo = notif.userInfo;
    //
    // Get keyboard animation.
    
    NSNumber *durationValue = userInfo[UIKeyboardAnimationDurationUserInfoKey];
    NSTimeInterval animationDuration = durationValue.doubleValue;
    
    NSNumber *curveValue = userInfo[UIKeyboardAnimationCurveUserInfoKey];
    UIViewAnimationCurve animationCurve = curveValue.intValue;
    
    if ([notif.name isEqualToString:UIKeyboardWillShowNotification])
    {
        NSValue *endFrameValue = userInfo[UIKeyboardFrameEndUserInfoKey];
        CGRect keyboardEndFrame = [self.view convertRect:endFrameValue.CGRectValue fromView:nil];
        
        CGFloat offset = kScreenHeight - keyboardEndFrame.origin.y + 100;
        _subtitleView.y = offset;
    }
    
    else if ([notif.name isEqualToString:UIKeyboardWillHideNotification])
    {
        _subtitleView.y = kScreenHeight - 170;
    }
    
    // Create animation.
    void (^animations)() = ^() {
        [self.view layoutIfNeeded];
    };
    
    [UIView animateWithDuration:animationDuration
                          delay:0.0
                        options:(animationCurve << 16)
                     animations:animations
                     completion:^(BOOL f){
                     }];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
