//
//  QHVCEditCommandManager.m
//  QHVideoCloudToolSet
//
//  Created by deng on 2017/12/28.
//  Copyright © 2017年 yangkui. All rights reserved.
//

#import "QHVCEditCommandManager.h"
#import "QHVCEditPhotoItem.h"
#import "QHVCEditStickerIconView.h"
#import "QHVCEditAudioItem.h"
#import "QHVCEditPrefs.h"
#import "QHVCEditSegmentItem.h"
#import "QHVCEditSubtitleItem.h"
#import "QHVCEditMatrixItem.h"
#import "QHVCEditOverlayItemPreview.h"

static QHVCEditCommandManager *manager = nil;

@interface QHVCEditCommandManager()
{
    
}
@property (nonatomic, strong) NSMutableArray<NSDictionary *> *commandsArray;

@end

@implementation QHVCEditCommandManager

+ (instancetype)manager {
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [[self alloc] init];
        [QHVCEditLog setLogLevel:QHVCEditLogLevel_Debug];
    });
    return manager;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        _commandsArray = [NSMutableArray array];
    }
    return self;
}

- (void)initCommandFactory
{
    [self freeCommandFactory];
    self.commandFactory = [[QHVCEditCommandFactory alloc] initCommandFactory];
    self.outputParams = [[QHVCEditOutputParams alloc] init];
    [self.outputParams setSize:CGSizeMake(kOutputVideoWidth, kOutputVideoHeight)];
    [self.outputParams setRenderMode:QHVCEditOutputRenderMode_AspectFit];
    self.commandFactory.defaultOutputParams = self.outputParams;
}

- (void)freeCommandFactory
{
    if (self.commandFactory)
    {
        [self.commandFactory freeCommandFactory];
        self.commandFactory = nil;
        [self.commandsArray removeAllObjects];
        [[QHVCEditPrefs sharedPrefs].matrixItems removeAllObjects];
    }
}

- (NSArray<NSDictionary *> *)fetchFiles
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddFile];
    return cs;
}

- (void)fetchSegmentInfo:(QHVCEditSegmentInfoBlock)complete
{
    [self.commandFactory getSegmentInfo:^(NSArray<QHVCEditSegmentInfo *> *segments, NSInteger totalDurationMs) {
        if (complete) {
            complete(segments,totalDurationMs);
        }
    }];
}

- (void)deleteFile:(NSInteger)fileIndex
{
    QHVCEditCommandDeleteFile *deleteCommand = [[QHVCEditCommandDeleteFile alloc] initCommand:self.commandFactory];
    deleteCommand.fileIndex = fileIndex;
    [deleteCommand addCommand];
}

- (void)addFiles:(NSArray<QHVCEditPhotoItem *> *)files
{
    NSArray<NSDictionary *> *cs = [self filterCommand:QHVCEditCommandOperationAddFile];
    NSInteger fileIndex = cs.count - 1;
    while (fileIndex >= 0) {
        [self deleteFile:fileIndex];
        fileIndex--;
    }

    if (cs.count > 0) {
        [self.commandsArray removeObjectsInArray:cs];
    }
    [self appendFiles:files];
}

- (void)appendFiles:(NSArray<QHVCEditPhotoItem *> *)files
{
    int i = 0;
    for (QHVCEditPhotoItem *item in files) {
        QHVCEditCommandAddFileSegment *addCommand = [[QHVCEditCommandAddFileSegment alloc] initCommand:self.commandFactory];
        addCommand.filePath = item.filePath;
        addCommand.fileIndex = i;
        addCommand.durationMs = item.asset.mediaType == PHAssetMediaTypeImage ? kImageFileDurationMS : 0;
        addCommand.startTimestampMs = item.startMs;
        addCommand.endTimestampMs = item.endMs;
        [addCommand addCommand];
        NSDictionary *cmd = [self formatCommand:QHVCEditCommandOperationAddFile object:addCommand status:QHVCEditCommandStatusAdd];
        [self.commandsArray addObject:cmd];
        i++;
    }
}

- (void)fetchThumbs:(NSString *)filePath start:(NSTimeInterval)startMs end:(NSTimeInterval)endMs frameCnt:(int)count thumbSize:(CGSize)size completion:(void (^)(NSArray<QHVCEditThumbnailItem *> *thumbnails))completion
{
    if (!_thumbnailFactory) {
        _thumbnailFactory = [[QHVCEditThumbnail alloc] initThumbnailFactory];
    }
    NSMutableArray *ts = [NSMutableArray array];
    [_thumbnailFactory getVideoThumbnailFromFile:filePath width:size.width height:size.height startTime:startMs endTime:endMs count:count callback:^(NSArray<QHVCEditThumbnailItem *> *thumbnails, QHVCEditThumbnailCallbackState state) {
        if ((state != QHVCEditThumbnailCallbackState_Error) || (state != QHVCEditThumbnailCallbackState_Cancel))
        {
            [ts addObjectsFromArray:thumbnails];
            if (ts.count >= count) {
                if (completion) {
                    completion(ts);
                }
            }
        }
    }];
}

- (void)addWatermask:(UIImage *)watermask
{
    NSTimeInterval start = 0.0;

    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddFile];
    for (NSDictionary *c in cs) {
        QHVCEditCommandAddFileSegment *cmd = (QHVCEditCommandAddFileSegment *)c[@"object"];
        QHVCEditCommandWaterMark *w = [[QHVCEditCommandWaterMark alloc] initCommand:self.commandFactory];
        w.image = watermask;
        w.destinationX = 10;
        w.destinationY = 10;
        w.destinationWidth = 200;
        w.destinationHeight = 30;
        w.destinationRotateAngle = 0;
        w.startTimestampMs = start;
        w.endTimestampMs =  w.startTimestampMs +cmd.endTimestampMs;
        [w addCommand];
        [self.commandsArray addObject:[self formatCommand:QHVCEditCommandOperationAddWatermask object:w status:QHVCEditCommandStatusAdd]];
        start = w.endTimestampMs;
    }
}

- (void)deleteWatermask;
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddWatermask];
    for (NSDictionary *c in cs) {
        QHVCEditCommandWaterMark *cmd = (QHVCEditCommandWaterMark *)c[@"object"];
        [cmd deleteCommand];
    }
    if (cs.count > 0) {
        [self.commandsArray removeObjectsInArray:cs];
    }
}

- (NSDictionary *)formatCommand:(QHVCEditCommandOperation)command object:(id)object status:(QHVCEditCommandStatus)status
{
    return @{@"command":@(command),
             @"object":object,
             @"status":@(status)
             };
}

- (NSArray<NSDictionary *> *)filterCommand:(QHVCEditCommandOperation)command
{
    if (self.commandsArray.count > 0) {
        NSPredicate *predicate = [NSPredicate predicateWithFormat:@"command =%@",@(command)];
        NSArray *cs = [self.commandsArray filteredArrayUsingPredicate:predicate];
        return cs;
    }
    return nil;
}

#pragma mark Quality

- (BOOL)adjustQuality:(NSInteger)type value:(float)value
{
    NSArray<NSDictionary *>* qs = [self filterCommand:QHVCEditCommandOperationAdjustQuality];
    if (qs.count > 0) {
        for (NSDictionary *q in qs) {
            QHVCEditCommandQualityFilter *cmd = (QHVCEditCommandQualityFilter *)q[@"object"];
            if (type == cmd.qualityType) {
                if (value != cmd.qualityValue) {
                    cmd.qualityValue = value;
                    [cmd editCommand];
                    return YES;
                }
               return NO;
            }
        }
    }
    NSTimeInterval start = 0.0;
    
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddFile];
    for (NSDictionary *c in cs) {
        QHVCEditCommandAddFileSegment *cmd = (QHVCEditCommandAddFileSegment *)c[@"object"];
        QHVCEditCommandQualityFilter *q = [[QHVCEditCommandQualityFilter alloc] initCommand:self.commandFactory];
        q.qualityType = type;
        q.qualityValue = value;
        q.startTimestampMs = start;
        q.endTimestampMs =  q.startTimestampMs +cmd.endTimestampMs;
        [q addCommand];
        [self.commandsArray addObject:[self formatCommand:QHVCEditCommandOperationAdjustQuality object:q status:QHVCEditCommandStatusAdd]];
        start = q.endTimestampMs;
    }
    return YES;
}

#pragma mark Speed
- (void)adjustSpeed:(float)speed
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddFile];
    for (NSInteger i = 0; i < cs.count; i++) {
        QHVCEditCommandAlterSpeed *command = [[QHVCEditCommandAlterSpeed alloc] initCommand:self.commandFactory];
        [command setFileIndex:i];
        [command setSpeed:speed];
        [command addCommand];
    }
}

#pragma mark Filter

- (void)addFilter:(NSDictionary *)item
{
    NSArray<NSDictionary *>* fs = [self filterCommand:QHVCEditCommandOperationAddFilter];
    if (fs.count > 0) {
        for (NSDictionary *f in fs) {
            QHVCEditCommandAuxFilter *cmd = (QHVCEditCommandAuxFilter *)f[@"object"];
            cmd.auxFilterType = [item[@"type"] intValue];
            cmd.auxFilterInfo = item[@"color"];
            [cmd editCommand];
            return;
        }
    }
    NSTimeInterval start = 0.0;
    
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddFile];
    for (NSDictionary *c in cs) {
        QHVCEditCommandAddFileSegment *cmd = (QHVCEditCommandAddFileSegment *)c[@"object"];
        QHVCEditCommandAuxFilter *f = [[QHVCEditCommandAuxFilter alloc] initCommand:self.commandFactory];
        f.auxFilterType = [item[@"type"] intValue];
        f.auxFilterInfo = item[@"color"];
        f.startTimestampMs = start;
        f.endTimestampMs =  f.startTimestampMs +cmd.endTimestampMs;
        [f addCommand];
        [self.commandsArray addObject:[self formatCommand:QHVCEditCommandOperationAddFilter object:f status:QHVCEditCommandStatusAdd]];
        start = f.endTimestampMs;
    }
}

#pragma mark Sticker

- (void)addSticker:(NSArray<QHVCEditStickerIconView *> *)stickers
{
    NSTimeInterval start = 0.0;
    NSTimeInterval end = 0.0;
    
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddFile];
    for (NSDictionary *c in cs) {
        QHVCEditCommandAddFileSegment *cmd = (QHVCEditCommandAddFileSegment *)c[@"object"];
        for (QHVCEditStickerIconView *sticker in stickers) {
            QHVCEditCommandImageFilter *f = [[QHVCEditCommandImageFilter alloc] initCommand:self.commandFactory];
            f.image = sticker.sticker.image;
            f.destinationX = sticker.frame.origin.x;
            f.destinationY = sticker.frame.origin.y;
            f.destinationWidth = sticker.sticker.frame.size.width;
            f.destinationHeight = sticker.sticker.frame.size.height;
            f.destinationRotateAngle = sticker.rotateAngle;
            f.startTimestampMs = start;
            f.endTimestampMs = f.startTimestampMs +cmd.endTimestampMs;
            end = f.endTimestampMs;
            [f addCommand];
        }
        start = end;
    }
}

#pragma mark Audio

- (void)addAudios:(NSArray<QHVCEditAudioItem *> *)audios
{
    for (QHVCEditAudioItem *item in audios) {
        if (item.audiofile.length > 0) {
            if (item.insertStartTimeMs >= item.insertEndTimeMs ||
                item.startTimeMs >= item.audioDuration) {
                continue;
            }
            QHVCEditCommandAudio *a = [[QHVCEditCommandAudio alloc]initCommand:self.commandFactory];
            NSString* filePath = [[NSBundle mainBundle] pathForResource:item.audiofile ofType:@"mp3"];
            if (!filePath) {
                filePath = [[NSBundle mainBundle] pathForResource:item.audiofile ofType:@"mp4"];
            }
            if (filePath.length <= 0) {
                continue;
            }
            a.filePath = filePath;
            a.startTime = item.startTimeMs;
            a.endTime = item.audioDuration;
            a.insertStartTime = item.insertStartTimeMs;
            a.insertEndTime = item.insertEndTimeMs;
            a.volume = item.volume;
            a.loop = (a.endTime - a.startTime) > (a.insertEndTime - a.insertStartTime)?NO:YES;
            [a addCommand];
            [self.commandsArray addObject:[self formatCommand:QHVCEditCommandOperationAddAudio object:a status:QHVCEditCommandStatusAdd]];
        }
        else
        {
            NSArray<NSDictionary *>* cs = [self fetchFiles];
            for (NSInteger i = 0; i < cs.count; i++) {
                QHVCEditCommandAlterVolume *cmd = [[QHVCEditCommandAlterVolume alloc]initCommand:self.commandFactory];
                cmd.fileIndex = i;
                cmd.volume = item.volume;
                [cmd addCommand];
            }
        }
    }
}

- (void)updateAudios
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddAudio];
    if (cs.count > 0) {
        [self.commandsArray removeObjectsInArray:cs];
    }
}

- (void)deleteAudios
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddAudio];
    for (NSDictionary *c in cs) {
        QHVCEditCommandAudio *cmd = (QHVCEditCommandAudio *)c[@"object"];
        [cmd deleteCommand];
    }
    if (cs.count > 0) {
        [self.commandsArray removeObjectsInArray:cs];
    }
}

#define kTransferDuration 2000

#pragma mark Transfer
- (void)addTransfers:(NSArray<QHVCEditSegmentItem *> *)transfers
{
    NSInteger i = 0;
    for (QHVCEditSegmentItem *item in transfers) {
        
        if(i+1 >= transfers.count)
        {
            return;
        }
        QHVCEditSegmentItem *originalSegment = transfers[i+1];

        if (item.transferType == QHVCEditTransferTypeNone) {
             [self deleteFile:i+1];
            
            QHVCEditCommandAddFileSegment *fileCommand = [[QHVCEditCommandAddFileSegment alloc] initCommand:self.commandFactory];
            fileCommand.filePath = originalSegment.filePath;
            fileCommand.fileIndex = i+1;
            fileCommand.durationMs = [self isFilePhoto:originalSegment.filePath] ? kImageFileDurationMS : 0;
            fileCommand.startTimestampMs = originalSegment.segmentStartTime;
            fileCommand.endTimestampMs = originalSegment.segmentEndTime;
            [fileCommand addCommand];
            
            [self recoverEffect:i+1];
        }
        else if(item.transferType != QHVCEditTransferTypeAdd)
        {
            [self deleteFile:i+1];
            
            QHVCEditCommandOverlaySegment *overlaySegment = [[QHVCEditCommandOverlaySegment alloc]initCommand:self.commandFactory];
            overlaySegment.filePath = originalSegment.filePath;
            overlaySegment.durationMs = [self isFilePhoto:originalSegment.filePath] ? kImageFileDurationMS : 0;
            overlaySegment.startTimestampMs = 0.0;
            overlaySegment.endTimestampMs = kTransferDuration;
            overlaySegment.insertTimestampMs = MIN(originalSegment.segmentStartTime - kTransferDuration, 0);
            [overlaySegment addCommand];
            
            if (originalSegment.fileDuration > kTransferDuration) {
                QHVCEditCommandAddFileSegment *fileCommand = [[QHVCEditCommandAddFileSegment alloc] initCommand:self.commandFactory];
                fileCommand.filePath = originalSegment.filePath;
                fileCommand.fileIndex = i+1;
                fileCommand.durationMs = overlaySegment.durationMs;
                fileCommand.startTimestampMs = kTransferDuration;
                fileCommand.endTimestampMs = originalSegment.fileDuration;
                [fileCommand addCommand];
                
                [self recoverEffect:i+1];
            }
            
            QHVCEditCommandTransition *transferCommand = [[QHVCEditCommandTransition alloc]initCommand:self.commandFactory];
            transferCommand.overlayCommandId = overlaySegment.commandId;
            transferCommand.transitionType = (QHVCEditTransitionType)item.transferType;
            transferCommand.startTimestampMs = MIN(originalSegment.segmentStartTime - kTransferDuration, 0) ;
            transferCommand.endTimestampMs = originalSegment.segmentStartTime;
            [transferCommand addCommand];
        }
        i++;
    }
}

- (BOOL)isFilePhoto:(NSString *)filePath
{
    return [QHVCEditGetFileInfo getFileInfo:filePath].isPicture;
}

#pragma mark Tailor

- (void)updateOutputSize:(CGSize)size
{
    [self.outputParams setSize:size];
}

- (void)updateOutputRenderMode:(QHVCEditOutputRenderMode)renderMode
{
    [self.outputParams setRenderMode:renderMode];
}

- (void)updateOutputBackgroudMode:(NSString *)colorHex
{
    if ([colorHex isEqualToString:@"blur"])
    {
        self.outputParams.backgroundMode = QHVCEditOutputBackgroundMode_Blur;
    }
    else
    {
        self.outputParams.backgroundMode = QHVCEditOutputBackgroundMode_Color;
        self.outputParams.backgroundInfo = colorHex;
    }
}

#pragma mark Subtitle

- (void)addSubtitles:(NSArray<QHVCEditSubtitleItem *> *)subtitles views:(NSArray<QHVCEditStickerIconView *> *)views
{
    NSInteger i = 0;
    for (QHVCEditSubtitleItem *item in subtitles) {
        
        QHVCEditStickerIconView *view = views[i];
        i++;
        if (item.insertStartTimeMs >= item.insertEndTimeMs) {
            continue;
        }
        if (item.composeImage) {
            QHVCEditCommandSubtitle *s = [[QHVCEditCommandSubtitle alloc]initCommand:self.commandFactory];
            s.image = item.composeImage;
            s.destinationX = view.frame.origin.x;
            s.destinationY = view.frame.origin.y;
            s.destinationWidth = view.sticker.frame.size.width;
            s.destinationHeight = view.sticker.frame.size.height;
            s.destinationRotateAngle = view.rotateAngle;
            s.startTimestampMs = item.insertStartTimeMs;
            s.endTimestampMs = item.insertEndTimeMs;
            [s addCommand];
            [self.commandsArray addObject:[self formatCommand:QHVCEditCommandOperationAddSubtitle object:s status:QHVCEditCommandStatusAdd]];
        }
    }
}

- (void)updateSubtitles
{
    
}

- (void)deleteSubtitles
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddSubtitle];
    for (NSDictionary *c in cs) {
        QHVCEditCommandSubtitle *cmd = (QHVCEditCommandSubtitle *)c[@"object"];
        [cmd deleteCommand];
    }
    if (cs.count > 0) {
        [self.commandsArray removeObjectsInArray:cs];
    }
}

- (void)recoverEffect:(NSInteger)index
{
    if (index == -1) {
        //recover all file effect
    }
    else
    {
        QHVCEditCommandOperation operation = QHVCEditCommandOperationAddWatermask;
        for (NSInteger i = operation; i < QHVCEditCommandOperationAddSubtitle; i++) {
            NSArray<NSDictionary *> *cs = [self filterCommand:i];
            if (i == QHVCEditCommandOperationAddAudio||
                i == QHVCEditCommandOperationAddSubtitle) {
                
                NSTimeInterval start = 0;
                NSTimeInterval end = 0;
                
                NSArray<NSDictionary *> *fs = [self filterCommand:QHVCEditCommandOperationAddFile];
                for (NSInteger i = 0;i <= index;i++) {
                    NSDictionary *f = fs[i];
                    QHVCEditCommandAddFileSegment *cmd = (QHVCEditCommandAddFileSegment *)f[@"object"];
                    start = end;
                    end = start + cmd.endTimestampMs - cmd.startTimestampMs;
                }
                for (NSDictionary *c in cs) {
                    QHVCEditCommandAudio *cmd = (QHVCEditCommandAudio *)c[@"object"];
                    if ((cmd.insertStartTime > start && cmd.insertStartTime < end)||
                        (cmd.insertEndTime > start && cmd.insertEndTime < end)) {
                        [cmd addCommand];
                    }
                }
            }
            if (cs.count > index) {
                QHVCEditCommand *cmd = cs[index][@"object"];
                [cmd addCommand];
            }
        }
    }
}

#pragma mark - Overlay

- (NSInteger)addOverlayFile:(QHVCEditPhotoItem *)file
{
    QHVCEditCommandOverlaySegment* cmd = [[QHVCEditCommandOverlaySegment alloc] initCommand:self.commandFactory];
    cmd.filePath = file.filePath;
    cmd.durationMs = file.endMs - file.startMs;
    cmd.startTimestampMs = file.asset.mediaType == PHAssetMediaTypeImage ? 0 : file.startMs;
    cmd.endTimestampMs = file.asset.mediaType == PHAssetMediaTypeImage ? 0 : file.endMs;
    cmd.insertTimestampMs = 0;
    [cmd addCommand];
    
    NSDictionary *command = [self formatCommand:QHVCEditCommandOperationAddOverlay object:cmd status:QHVCEditCommandStatusAdd];
    [self.commandsArray addObject:command];
    
    return cmd.commandId;
}

- (void)updateOverlayFile:(QHVCEditPhotoItem *)file overlayId:(NSInteger)overlayId
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddOverlay];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
    {
        QHVCEditCommandOverlaySegment *cmd = (QHVCEditCommandOverlaySegment *)obj[@"object"];
        if (cmd.commandId == overlayId)
        {
            cmd.filePath = file.filePath;
            cmd.durationMs = file.endMs - file.startMs;
            cmd.startTimestampMs = file.asset.mediaType == PHAssetMediaTypeImage ? 0 : file.startMs;
            cmd.endTimestampMs = file.asset.mediaType == PHAssetMediaTypeImage ? 0 : file.endMs;
            cmd.insertTimestampMs = 0;
            [cmd editCommand];
            *stop = YES;
        }
    }];
}

- (void)deleteOverlayFile:(NSInteger)overlayId
{
    __block NSDictionary* command = nil;
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddOverlay];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         QHVCEditCommandOverlaySegment *cmd = (QHVCEditCommandOverlaySegment *)obj[@"object"];
         if (cmd.commandId == overlayId)
         {
             command = obj;
             [cmd deleteCommand];
             *stop = YES;
         }
     }];
    
    [self.commandsArray removeObject:command];
}

- (void)updateOverlaySpeed:(CGFloat)speed overlayId:(NSInteger)overlayId
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddOverlay];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         QHVCEditCommandOverlaySegment *cmd = (QHVCEditCommandOverlaySegment *)obj[@"object"];
         if (cmd.commandId == overlayId)
         {
             cmd.speed = speed;
             [cmd editCommand];
             *stop = YES;
         }
     }];
}

- (void)updateOverlayVolume:(CGFloat)volume overlayId:(NSInteger)overlayId
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddOverlay];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         QHVCEditCommandOverlaySegment *cmd = (QHVCEditCommandOverlaySegment *)obj[@"object"];
         if (cmd.commandId == overlayId)
         {
             cmd.volume = volume;
             [cmd editCommand];
             *stop = YES;
         }
     }];
}

#pragma mark - Matrix

- (void)addMatrix:(QHVCEditMatrixItem *)item
{
    if (item.overlayCommandId == kMainTrackId)
    {
        NSTimeInterval start = 0.0;
        NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddFile];
        for (NSDictionary *c in cs) {
            QHVCEditCommandAddFileSegment *cmd = (QHVCEditCommandAddFileSegment *)c[@"object"];
            QHVCEditCommandMatrixFilter *f = [[QHVCEditCommandMatrixFilter alloc] initCommand:self.commandFactory];
            f.overlayCommandId = kMainTrackId;
            f.frameRotateAngle = item.frameRadian;
            f.flipX = item.flipX;
            f.flipY = item.flipY;
            f.renderRect = item.renderRect;
            f.sourceRect = item.sourceRect;
            f.renderZOrder = item.zOrder;
            f.preview = (UIView *)item.preview;
            f.outputParams = item.outputParams;
            f.startTimestampMs = start;
            f.endTimestampMs =  f.startTimestampMs + (cmd.endTimestampMs - cmd.startTimestampMs);
            [f addCommand];
            
            [self.commandsArray addObject:[self formatCommand:QHVCEditCommandOperationAddMatrix object:f status:QHVCEditCommandStatusAdd]];
            start = f.endTimestampMs;
        }
    }
    else
    {
        QHVCEditCommandMatrixFilter* cmd = [[QHVCEditCommandMatrixFilter alloc] initCommand:self.commandFactory];
        cmd.overlayCommandId = item.overlayCommandId;
        cmd.frameRotateAngle = item.frameRadian;
        cmd.previewRotateAngle = item.previewRadian;
        cmd.flipX = item.flipX;
        cmd.flipY = item.flipY;
        cmd.renderRect = item.renderRect;
        cmd.sourceRect = item.sourceRect;
        cmd.renderZOrder = item.zOrder;
        cmd.preview = item.preview.overlay;
        cmd.outputParams = item.outputParams;
        cmd.startTimestampMs = item.startTimestampMs;
        cmd.endTimestampMs = item.endTiemstampMs;
        [cmd addCommand];
        
        NSDictionary *command = [self formatCommand:QHVCEditCommandOperationAddMatrix object:cmd status:QHVCEditCommandStatusAdd];
        [self.commandsArray addObject:command];
    }
}

- (void)updateMatrix:(QHVCEditMatrixItem *)item
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddMatrix];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         QHVCEditCommandMatrixFilter *cmd = (QHVCEditCommandMatrixFilter *)obj[@"object"];
         if (cmd.overlayCommandId == item.overlayCommandId)
         {
             cmd.frameRotateAngle = item.frameRadian;
             cmd.previewRotateAngle = item.previewRadian;
             cmd.flipX = item.flipX;
             cmd.flipY = item.flipY;
             cmd.renderRect = item.renderRect;
             cmd.sourceRect = item.sourceRect;
             cmd.renderZOrder = item.zOrder;
             cmd.preview = item.preview.overlay;
             cmd.outputParams = item.outputParams;
             if (item.overlayCommandId != kMainTrackId)
             {
                 cmd.startTimestampMs = item.startTimestampMs;
                 cmd.endTimestampMs = item.endTiemstampMs;
                 *stop = YES;
             }
             [cmd editCommand];
         }
     }];
}

- (void)deleteMatrix:(QHVCEditMatrixItem *)item
{
    __block NSDictionary* command = nil;
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddMatrix];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         QHVCEditCommandMatrixFilter *cmd = (QHVCEditCommandMatrixFilter *)obj[@"object"];
         if (cmd.overlayCommandId == item.overlayCommandId)
         {
             command = obj;
             [cmd deleteCommand];
             if (item.overlayCommandId != kMainTrackId)
             {
                *stop = YES;
             }
         }
     }];
    
    [self.commandsArray removeObject:command];
}

- (void)addChromakeyWithColor:(NSString*)argb
                    threshold:(int)threshold
                       extend:(int)extend
             overlayCommandId:(NSInteger)overlayCommandId
             startTimestampMs:(int)startTimestampMs
               endTimestampMs:(int)endTimestampMs
{
    QHVCEditCommandChromakey* cmd = [[QHVCEditCommandChromakey alloc] initCommand:self.commandFactory];
    cmd.overlayCommandId = overlayCommandId;
    cmd.color = argb;
    cmd.threshold = threshold;
    cmd.extend = extend;
    cmd.startTimestampMs = startTimestampMs;
    cmd.endTimestampMs = endTimestampMs;
    [cmd addCommand];
    
    NSDictionary *command = [self formatCommand:QHVCEditCommandOperationAddChromakey object:cmd status:QHVCEditCommandStatusAdd];
    [self.commandsArray addObject:command];
}

- (void)updateChromakeyWithColor:(NSString*)argb threshold:(int)threshold extend:(int)extend overlayCommandId:(NSInteger)overlayCommandId
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddChromakey];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         QHVCEditCommandChromakey *cmd = (QHVCEditCommandChromakey *)obj[@"object"];
         if (cmd.overlayCommandId == overlayCommandId)
         {
             cmd.color = argb;
             cmd.threshold = threshold;
             cmd.extend = extend;
             [cmd editCommand];
             *stop = YES;
         }
     }];
}

- (void)deleteChromakey:(NSInteger)overlayCommandId
{
    NSArray<NSDictionary *>* cs = [self filterCommand:QHVCEditCommandOperationAddChromakey];
    [cs enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop)
     {
         QHVCEditCommandChromakey *cmd = (QHVCEditCommandChromakey *)obj[@"object"];
         if (cmd.overlayCommandId == overlayCommandId)
         {
             [cmd deleteCommand];
             *stop = YES;
         }
     }];
}

@end
