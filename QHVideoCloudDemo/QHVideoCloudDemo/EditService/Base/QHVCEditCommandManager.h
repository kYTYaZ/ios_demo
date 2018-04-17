//
//  QHVCEditCommandManager.h
//  QHVideoCloudToolSet
//
//  Created by deng on 2017/12/28.
//  Copyright © 2017年 yangkui. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QHVCEditKit/QHVCEditKit.h>

@class QHVCEditPhotoItem;
@class QHVCEditStickerIconView;
@class QHVCEditAudioItem;
@class QHVCEditSegmentItem;
@class QHVCEditSubtitleItem;
@class QHVCEditMatrixItem;

typedef NS_ENUM(NSInteger, QHVCEditCommandStatus) {
    QHVCEditCommandStatusAdd = 1,
    QHVCEditCommandStatusDone,
};

typedef NS_ENUM(NSInteger, QHVCEditCommandOperation) {
    QHVCEditCommandOperationAddFile = 1,
    QHVCEditCommandOperationAddWatermask,
    QHVCEditCommandOperationAdjustQuality,
    QHVCEditCommandOperationAddFilter,
    QHVCEditCommandOperationAddMatrix,
    QHVCEditCommandOperationAddAudio,
    QHVCEditCommandOperationAddSubtitle,
    QHVCEditCommandOperationAddOverlay,
    QHVCEditCommandOperationAddChromakey,
};

@interface QHVCEditCommandManager : NSObject

@property (nonatomic, strong) QHVCEditCommandFactory *commandFactory;
@property (nonatomic, strong) QHVCEditOutputParams* outputParams;
@property (nonatomic, strong) QHVCEditThumbnail* thumbnailFactory;

+ (instancetype)manager;

- (void)initCommandFactory;
- (void)freeCommandFactory;

- (void)addFiles:(NSArray<QHVCEditPhotoItem *> *)files;
- (void)appendFiles:(NSArray<QHVCEditPhotoItem *> *)files;
- (NSArray<NSDictionary *> *)fetchFiles;

- (void)fetchThumbs:(NSString *)filePath start:(NSTimeInterval)startMs end:(NSTimeInterval)endMs frameCnt:(int)count thumbSize:(CGSize)size completion:(void (^)(NSArray<QHVCEditThumbnailItem *> *thumbnails))completion;
- (void)fetchSegmentInfo:(QHVCEditSegmentInfoBlock)complete;

- (void)addWatermask:(UIImage *)watermask;
- (void)deleteWatermask;

- (BOOL)adjustQuality:(NSInteger)type value:(float)value;

- (void)adjustSpeed:(float)speed;

- (void)addFilter:(NSDictionary *)item;

- (void)addSticker:(NSArray<QHVCEditStickerIconView *> *)stickers;

- (void)addAudios:(NSArray<QHVCEditAudioItem *> *)audios;
- (void)updateAudios;
- (void)deleteAudios;

- (void)addTransfers:(NSArray<QHVCEditSegmentItem *> *)transfers;

- (void)updateOutputSize:(CGSize)size;
- (void)updateOutputRenderMode:(QHVCEditOutputRenderMode)renderMode;
- (void)updateOutputBackgroudMode:(NSString *)colorHex;

- (void)addSubtitles:(NSArray<QHVCEditSubtitleItem *> *)subtitles views:(NSArray<QHVCEditStickerIconView *> *)views;
- (void)updateSubtitles;
- (void)deleteSubtitles;

- (NSInteger)addOverlayFile:(QHVCEditPhotoItem *)file;
- (void)updateOverlayFile:(QHVCEditPhotoItem *)file overlayId:(NSInteger)overlayId;
- (void)deleteOverlayFile:(NSInteger)overlayId;
- (void)updateOverlaySpeed:(CGFloat)speed overlayId:(NSInteger)overlayId;
- (void)updateOverlayVolume:(CGFloat)volume overlayId:(NSInteger)overlayId;

- (void)addMatrix:(QHVCEditMatrixItem *)item;
- (void)updateMatrix:(QHVCEditMatrixItem *)item;
- (void)deleteMatrix:(QHVCEditMatrixItem *)item;

- (void)addChromakeyWithColor:(NSString*)argb
                    threshold:(int)threshold
                       extend:(int)extend
             overlayCommandId:(NSInteger)overlayCommandId
             startTimestampMs:(int)startTimestampMs
               endTimestampMs:(int)endTimestampMs;

- (void)updateChromakeyWithColor:(NSString*)argb
                       threshold:(int)threshold
                          extend:(int)extend
                overlayCommandId:(NSInteger)overlayCommandId;

- (void)deleteChromakey:(NSInteger)overlayCommandId;

@end
