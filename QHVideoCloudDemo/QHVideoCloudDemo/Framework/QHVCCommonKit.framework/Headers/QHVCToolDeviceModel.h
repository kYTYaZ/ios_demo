
#import <Foundation/Foundation.h>

#define kQHVCToolDeviceRank_Baseline                         10
#define kQHVCToolDeviceRank_AppleA4Class                     20   // Cortex-A8 class
#define kQHVCToolDeviceRank_AppleA5Class                     30   // Cortex-A9 class
#define kQHVCToolDeviceRank_AppleA5RAClass                   31   // Cortex-A9 class
#define kQHVCToolDeviceRank_AppleA5XClass                    35   // Cortex-A9 class
#define kQHVCToolDeviceRank_AppleA6Class                     40   // ARMv7s class
#define kQHVCToolDeviceRank_AppleA6XClass                    41   // ARMv7s class
#define kQHVCToolDeviceRank_AppleA7Class                     50   // ARM64 class
#define kQHVCToolDeviceRank_AppleA8LowClass                  55   // ARM64 class
#define kQHVCToolDeviceRank_AppleA8Class                     60   // ARM64 class
#define kQHVCToolDeviceRank_AppleA8XClass                    61   // ARM64 class
#define kQHVCToolDeviceRank_AppleA9Class                     70   // ARM64 class
#define kQHVCToolDeviceRank_AppleA9XClass                    71   // ARM64 class
#define kQHVCToolDeviceRank_AppleA10Class                    72   // ARM64 class
#define kQHVCToolDeviceRank_LatestUnknown                    90
#define kQHVCToolDeviceRank_Simulator                        100

@interface QHVCToolDeviceModel : NSObject

@property(nonatomic) NSString   *platform;
@property(nonatomic) NSString   *name;
@property(nonatomic) NSInteger  rank;


/**
 获取当前设备信息

 @return QHVCToolDeviceModel对象
 */
+ (instancetype)currentModel;


/**
 获取操作系统名称

 @return 操作系统名称
 */
+ (NSString *)getSystemName;


/**
 获取操作系统版本号

 @return 操作系统版本号
 */
+ (NSString *)getSystemVersion;


/**
 获取设备的UUID，唯一，只要使用视频云SDK，UUID都是唯一的
 
 @return 设备UUID
 */
+ (NSString *)getDeviceUUID;


/**
 判断设备是否低于某个指定的型号

 @param modelPlatform 设备型号
 @return YES：低于指定设备，NO：高于指定设备
 */
+ (BOOL)isDeviceBelowModel:(NSString *)modelPlatform;

@end
