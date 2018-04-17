//
//  QHVCEditMainViewController.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2017/12/29.
//  Copyright © 2017年 yangkui. All rights reserved.
//

#import "QHVCEditMainViewController.h"
#import "QHVCEditPrefs.h"
#import "QHVCEditMainNavView.h"
#import "QHVCEditMakeViewController.h"

typedef NS_ENUM(NSUInteger, QHVCEditVCType)
{
    QHVCEditVCType_PlayerVC,
    QHVCEditVCType_ResourceVC,
    QHVCEditVCType_BaseVC,
};

@interface QHVCEditMainViewController ()
{
    NSArray<NSArray*> *_functions;
    QHVCEditMainNavView *_navView;
}
@end

@implementation QHVCEditMainViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.titleLabel.text = @"编辑";
    _functions = @[@[@"滤镜",@"edit_filter",@"QHVCEditAuxFilterViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"剪辑",@"edit_tailor",@"QHVCEditTailorViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"字幕",@"edit_caption",@"QHVCEditSubtitleViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"贴纸",@"edit_sticker",@"QHVCEditStickerViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"倍速",@"edit_speed",@"QHVCEditSpeedViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"调节",@"edit_adjust",@"QHVCEditQualityViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"音乐",@"edit_audio",@"QHVCEditAudioViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"水印",@"edit_water",@"QHVCEditWatermaskViewController", @(QHVCEditVCType_PlayerVC)],
                   @[@"转场",@"edit_transfer",@"QHVCEditTransferViewController", @(QHVCEditVCType_PlayerVC)],
                 /*  @[@"分段",@"edit_cut",@"QHVCEditCutViewController"]*/
                   @[@"画中画",@"edit_overlay",@"QHVCEditOverlayResourceVC", @(QHVCEditVCType_ResourceVC)]];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self createNavView];
}

- (void)createNavView
{
    if (!_navView) {
        _navView = [[NSBundle mainBundle] loadNibNamed:[[QHVCEditMainNavView class] description] owner:self options:nil][0];
        _navView.frame = CGRectMake(0, kScreenHeight - 100, kScreenWidth, 100);
        [_navView updateView:_functions];
        __weak typeof(self) weakSelf = self;
        _navView.selectedCompletion = ^(NSInteger index) {
            [weakSelf navToFunction:index];
        };
        [self.view addSubview:_navView];
    }
}

- (void)nextAction:(UIButton *)btn
{
    QHVCEditMakeViewController *vc = [[QHVCEditMakeViewController alloc]init];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)backAction:(UIButton *)btn
{
    [self showAlert];
}

- (void)navToFunction:(NSInteger)index
{
    NSArray *item = _functions[index];
    if (item.count > 3)
    {
        NSString *className = item[2];
        QHVCEditVCType type = (QHVCEditVCType)[item[3] integerValue];
        if (type == QHVCEditVCType_PlayerVC)
        {
            QHVCEditPlayerMainViewController *vc = [[NSClassFromString(className) alloc] initWithNibName:@"QHVCEditPlayerMainViewController" bundle:nil];
            vc.durationMs = self.durationMs;
            __weak typeof(self) weakSelf = self;
            vc.confirmCompletion = ^(QHVCEditPlayerStatus status) {
                if (status == QHVCEditPlayerStatusRefresh) {
                    [weakSelf refreshPlayer];
                }
                else if (status == QHVCEditPlayerStatusReset)
                {
                    [weakSelf resetPlayer:self.durationMs];
                }
            };
            [self.navigationController pushViewController:vc animated:YES];
        }
        else if (type == QHVCEditVCType_ResourceVC)
        {
            UIViewController* vc = [[NSClassFromString(className) alloc] initWithNibName:@"QHVCEditViewController" bundle:nil];
            [self.navigationController pushViewController:vc animated:YES];
        }
        else
        {
            UIViewController* vc = [[NSClassFromString(className) alloc] initWithNibName:className bundle:nil];
            [self.navigationController pushViewController:vc animated:YES];
        }
    }
}

- (void)showAlert {
    
    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"放弃操作么" message:@"" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:@"取消", nil];
    [alert show];
}

#pragma mark UIAlertViewDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 0) {
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
