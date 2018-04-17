//
//  QHVCEditPhotoManage.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2017/12/22.
//  Copyright © 2017年 yangkui. All rights reserved.
//

#import "QHVCEditPhotoManager.h"
#import "QHVCEditPrefs.h"

@interface  QHVCEditPhotoManager()<UIAlertViewDelegate>
{
    PHCachingImageManager *_cacheImageManager;
}
@end

static QHVCEditPhotoManager *singleInstance = nil;

@implementation QHVCEditPhotoManager

+ (instancetype)manager
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        singleInstance = [[self alloc] init];
    });
    return singleInstance;
}

- (instancetype) init
{
    self = [super init];
    if (self) {
        _cacheImageManager = [[PHCachingImageManager alloc] init];
    }
    return self;
}

- (void)startImageCache:(NSArray *)photos targetSize:(CGSize)thumbSize
{
    [_cacheImageManager startCachingImagesForAssets:photos targetSize:thumbSize contentMode:PHImageContentModeAspectFill options:nil];
}

- (void)stopImageCache:(NSArray *)photos targetSize:(CGSize)thumbSize
{
    [_cacheImageManager stopCachingImagesForAssets:photos targetSize:thumbSize contentMode:PHImageContentModeAspectFill options:nil];
}

- (void)fetchPhotos:(void(^)(NSArray<QHVCEditPhotoItem *> *photos,NSArray<PHAsset *> *caches))completion
{
    PHFetchResult* albums = [PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeSmartAlbum subtype:PHAssetCollectionSubtypeAlbumRegular options:nil];
    [albums enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         PHAssetCollection* collection = obj;
         if ([collection.localizedTitle isEqualToString:@"相机胶卷"] ||
             [collection.localizedTitle isEqualToString:@"Camera Roll"] ||
             [collection.localizedTitle isEqualToString:@"All Photos"]||
             [collection.localizedTitle isEqualToString:@"所有照片"])
         {
             NSMutableArray *photos = [NSMutableArray array];
             NSMutableArray *caches = [NSMutableArray array];
             
             PHFetchOptions* option = [[PHFetchOptions alloc] init];
             option.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"creationDate" ascending:NO]];
             PHFetchResult<PHAsset *>* assets = [PHAsset fetchAssetsInAssetCollection:collection options:option];
             [assets enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
              {
                  PHAsset* asset = obj;
                  QHVCEditPhotoItem *item = [[QHVCEditPhotoItem alloc]init];
                  item.asset = asset;
                  item.startMs = 0;
                  item.endMs = (item.asset.mediaType == PHAssetMediaTypeImage) ? kImageFileDurationMS:item.asset.duration*1000.0;
                  item.lastStartMs = item.startMs;
                  item.lastEndMs = item.endMs;
                  item.durationMs = item.endMs;
                  [photos addObject:item];
                  [caches addObject:asset];
              }];
             if (completion) {
                 completion(photos,caches);
             }
         }
     }];
}

- (void)fetchImageForAsset:(QHVCEditPhotoItem *)item options:(nullable PHImageRequestOptions *)options completion:(void (^)(void))completion
{
    [_cacheImageManager requestImageForAsset:item.asset
                                        targetSize:item.thumbImageCacheSize
                                       contentMode:PHImageContentModeAspectFill
                                           options:nil
                                     resultHandler:^(UIImage * _Nullable result, NSDictionary * _Nullable info)
     {
         item.thumbImage = result;
         item.info = info;
         if (completion) {
             completion();
         }
     }];
}

- (void)writeAssetsToSandbox:(NSArray<QHVCEditPhotoItem *> *)items
{
   NSString *cacheDir = [self assetCacheDir];
    if (!cacheDir) {
        return;
    }
    NSInteger i = 0;
    for (QHVCEditPhotoItem *item in items) {
        item.sortIndex = i;
        i++;
        if (item.filePath) {
            continue;
        }
        PHAssetResource * resource = [[PHAssetResource assetResourcesForAsset:item.asset] firstObject];
        NSString *path = [cacheDir stringByAppendingPathComponent:resource.originalFilename];
        item.filePath = path;
        if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
            continue;
        }
        [[PHAssetResourceManager defaultManager] writeDataForAssetResource:resource toFile:[NSURL fileURLWithPath:path] options:nil completionHandler:^(NSError * _Nullable error) {
            if (error) {
                NSLog(@"writeDataForAssetResource fail %@",[NSThread currentThread].name);
            }
            else
            {
                NSLog(@"writeDataForAssetResource sucess %@",[NSThread currentThread].name);
            }
        }];
    }
}

- (NSString *)assetCacheDir
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    NSString *assetFilePath = [NSString stringWithFormat:@"%@/%@",cachesDir,@"photo"];
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:assetFilePath isDirectory:nil])
    {
        BOOL isSucess = [[NSFileManager defaultManager] createDirectoryAtPath:assetFilePath withIntermediateDirectories:YES attributes:nil error:nil];
        if (!isSucess) {
            return nil;
        }
    }
    return assetFilePath;
}

- (NSString *)videoTempDir
{
    NSString *videoFilePath = [NSString stringWithFormat:@"%@%@",NSTemporaryDirectory(),@"video"];
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:videoFilePath isDirectory:nil])
    {
        BOOL isSucess = [[NSFileManager defaultManager] createDirectoryAtPath:videoFilePath withIntermediateDirectories:YES attributes:nil error:nil];
        if (!isSucess) {
            return nil;
        }
    }
    return videoFilePath;
}

- (void)saveVideoToAlbum:(NSString *)sourcePath
{
    if (UIVideoAtPathIsCompatibleWithSavedPhotosAlbum(sourcePath)) {

        UISaveVideoAtPathToSavedPhotosAlbum(sourcePath, self,
                                            @selector(video:didFinishSavingWithError:contextInfo:), nil);
    }
}

- (void)video:(NSString *)videoPath didFinishSavingWithError:(NSError *)error
  contextInfo:(void *)contextInfo {
    NSLog(@"saveVideoToAlbum done");
}

#pragma mark - 相册授权
- (BOOL)isPhotoAuthorized {
    if ([PHPhotoLibrary authorizationStatus] == PHAuthorizationStatusAuthorized) {
        return YES;
    }
    return NO;
}

- (void)requestAuthorization:(void(^)(BOOL granted))completion {
    if (![self isPhotoAuthorized]) {
        [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
            dispatch_async(dispatch_get_main_queue(), ^{
                if (status != PHAuthorizationStatusAuthorized) {
                    [self showAlert];
                    completion(NO);
                }
                else {
                    completion(YES);
                }
            });
        }];
    }
    else {
        completion(YES);
    }
}

- (void)showAlert {
    
    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"需要访问你的相册" message:@"请授权相册访问权限" delegate:self cancelButtonTitle:@"设置" otherButtonTitles:@"取消", nil];
    [alert show];
}

#pragma mark UIAlertViewDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 0) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
    }
}

@end
