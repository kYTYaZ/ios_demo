//
//  QHVCEditWatermaskViewController.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2018/1/4.
//  Copyright © 2018年 yangkui. All rights reserved.
//

#import "QHVCEditWatermaskViewController.h"
#import "UIViewAdditions.h"
#import "QHVCEditPrefs.h"
#import "QHVCEditCommandManager.h"

@interface QHVCEditWatermaskViewController ()
{
    UIButton *_watermaskBtn;
    BOOL _watermaskStatus;
}
@end

@implementation QHVCEditWatermaskViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.titleLabel.text = @"水印";
    [self.nextBtn setTitle:@"确定" forState:UIControlStateNormal];
    
    _watermaskStatus = [QHVCEditPrefs sharedPrefs].isEnableWatermsk;

    [self createWatermaskView];
}

- (void)createWatermaskView
{
    _watermaskBtn = [[UIButton alloc]initWithFrame:CGRectMake(0, kScreenHeight - 100, kScreenWidth, 100)];
    [_watermaskBtn addTarget:self action:@selector(watermaskAction:) forControlEvents:UIControlEventTouchUpInside];
    [_watermaskBtn setBackgroundColor:[UIColor blackColor]];
    NSString *text = [QHVCEditPrefs sharedPrefs].isEnableWatermsk?@"关闭":@"开启";
    [_watermaskBtn setTitle:[NSString stringWithFormat:@"%@水印",text] forState:UIControlStateNormal];
    [_watermaskBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [self.view addSubview:_watermaskBtn];
}

- (void)watermaskAction:(UIButton *)sender
{
    if ([QHVCEditPrefs sharedPrefs].isEnableWatermsk) {
        [_watermaskBtn setTitle:@"开启水印" forState:UIControlStateNormal];
        [QHVCEditPrefs sharedPrefs].isEnableWatermsk = NO;
        [[QHVCEditCommandManager manager] deleteWatermask];
    }
    else
    {
        [_watermaskBtn setTitle:@"关闭水印" forState:UIControlStateNormal];
        [QHVCEditPrefs sharedPrefs].isEnableWatermsk = YES;
        [[QHVCEditCommandManager manager] addWatermask:[UIImage imageNamed:@"edit_watermask"]];
    }
    [self refreshPlayer];
}

- (void)nextAction:(UIButton *)btn
{
    if (_watermaskStatus != [QHVCEditPrefs sharedPrefs].isEnableWatermsk) {
        if (self.confirmCompletion) {
            self.confirmCompletion(QHVCEditPlayerStatusRefresh);
        }
    }
    [self releasePlayerVC];
}

- (void)backAction:(UIButton *)btn
{
    if (_watermaskStatus != [QHVCEditPrefs sharedPrefs].isEnableWatermsk) {
        if (_watermaskStatus) {
            [[QHVCEditCommandManager manager] addWatermask:[UIImage imageNamed:@"edit_watermask"]];
        }
        else
        {
            [[QHVCEditCommandManager manager] deleteWatermask];
        }
        [QHVCEditPrefs sharedPrefs].isEnableWatermsk = _watermaskStatus;
    }
    [self releasePlayerVC];
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
