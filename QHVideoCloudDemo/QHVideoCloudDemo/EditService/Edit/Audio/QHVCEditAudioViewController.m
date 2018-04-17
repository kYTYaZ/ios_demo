//
//  QHVCEditAudioViewController.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2018/1/18.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import "QHVCEditAudioViewController.h"
#import "QHVCEditAddAudioView.h"
#import "QHVCEditFrameView.h"
#import "QHVCEditPrefs.h"
#import "QHVCEditAudioItem.h"
#import "QHVCEditCommandManager.h"

@interface QHVCEditAudioViewController ()
{
    QHVCEditFrameView *_frameView;
    QHVCEditAddAudioView *_audioView;
    QHVCEditFrameStatus _viewType;
    float _originAudioVolume;
}
@property (nonatomic, strong) QHVCEditAudioItem *currentAudioItem;
@property (nonatomic, strong) NSMutableArray<QHVCEditAudioItem *> *audiosArray;
@property (nonatomic, strong) NSMutableArray<NSMutableArray *> *audioInfos;

@end

@implementation QHVCEditAudioViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.titleLabel.text = @"音乐";
    [self.nextBtn setTitle:@"确定" forState:UIControlStateNormal];
    _originAudioVolume = [QHVCEditPrefs sharedPrefs].originAudioVolume;
    _audiosArray = [NSMutableArray array];
    _audioInfos = [NSMutableArray arrayWithArray:[QHVCEditPrefs sharedPrefs].audioTimestamp];
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
    _frameView.timeStamp = [QHVCEditPrefs sharedPrefs].audioTimestamp;
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
    
    _currentAudioItem = [[QHVCEditAudioItem alloc] init];
    _currentAudioItem.insertStartTimeMs = insertStartMs;
    
    _audioView = [[NSBundle mainBundle] loadNibNamed:[[QHVCEditAddAudioView class] description] owner:self options:nil][0];
    _audioView.frame = CGRectMake(0, kScreenHeight - 200, kScreenWidth, 200);
    _audioView.audioItem = self.currentAudioItem;
    [self.view addSubview:_audioView];
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
    [_audioView removeFromSuperview];
    _audioView = nil;
    
    [self updateViewType:QHVCEditFrameStatusAdd];
}

- (void)updateViewType:(QHVCEditFrameStatus)status
{
    _viewType = status;
    
    if (status == QHVCEditFrameStatusAdd) {
        self.titleLabel.text = @"音乐";
        [_frameView setUIStatus:_viewType];
        _frameView.hidden = NO;
        _sliderViewBottom.constant = 70;
        [_frameView removeUncompleteTimestamp];
    }
    else if (status == QHVCEditFrameStatusEdit)
    {
        self.titleLabel.text = @"选择";
        _frameView.hidden = YES;
        _sliderViewBottom.constant = 100;
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
        if (_currentAudioItem.audiofile.length > 0) {
            _audioView.hidden = YES;
            
            [self updateViewType:QHVCEditFrameStatusDone];
        }
        else
        {
            [_audioView removeFromSuperview];
            _audioView = nil;

            [self updateViewType:QHVCEditFrameStatusAdd];
            
            if (![_audiosArray containsObject:_currentAudioItem]) {
                [_audiosArray addObject:_currentAudioItem];
            }
            [[QHVCEditCommandManager manager] addAudios:@[_currentAudioItem]];
            [self resetPlayer:[_frameView fetchCurrentTimeStampMs]];
        }
    }
    else if (_viewType == QHVCEditFrameStatusDone)
    {
        [_audioView removeFromSuperview];
        _audioView = nil;

        [self updateViewType:QHVCEditFrameStatusAdd];
        
        self.currentAudioItem.insertEndTimeMs = [_frameView fetchCurrentTimeStampMs];
        if (![_audiosArray containsObject:_currentAudioItem]) {
            [_audiosArray addObject:_currentAudioItem];
        }
        [[QHVCEditCommandManager manager] addAudios:@[_currentAudioItem]];
        [self resetPlayer:[_frameView fetchCurrentTimeStampMs]];
    }
    else
    {
        if (_audiosArray.count > 0) {
            if (self.confirmCompletion) {
                self.confirmCompletion(QHVCEditPlayerStatusReset);
            }
            [[QHVCEditCommandManager manager] updateAudios];
        }
        [self releasePlayerVC];
    }
}

- (void)backAction:(UIButton *)btn
{
    if(_viewType == QHVCEditFrameStatusEdit)
    {
        [_audioView removeFromSuperview];
        _audioView = nil;

        [self updateViewType:QHVCEditFrameStatusAdd];
        [QHVCEditPrefs sharedPrefs].originAudioVolume = _originAudioVolume;
    }
    else if (_viewType == QHVCEditFrameStatusDone)
    {
        _audioView.hidden = NO;
       
        [self updateViewType:QHVCEditFrameStatusEdit];
    }
    else
    {
        if ([QHVCEditPrefs sharedPrefs].originAudioVolume != _originAudioVolume) {
            QHVCEditAudioItem *item = [[QHVCEditAudioItem alloc] init];
            item.volume = _originAudioVolume;
            [[QHVCEditCommandManager manager] addAudios:@[item]];
            [QHVCEditPrefs sharedPrefs].originAudioVolume = _originAudioVolume;
        }
        if (_audiosArray.count > 0) {
            [[QHVCEditCommandManager manager] deleteAudios];
        }
        [QHVCEditPrefs sharedPrefs].audioTimestamp = _audioInfos;
        [self releasePlayerVC];
    }
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
