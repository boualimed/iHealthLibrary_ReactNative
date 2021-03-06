//
//  HS4SModule.m
//  ReactNativeIOSLibrary
//
//  Created by ihealth on 16/12/2.
//  Copyright © 2016年 daiqingquan. All rights reserved.
//

#import "HS4SModule.h"
#import "HSProfileModule.h"
#import "HSMacroFile.h"
#import "HS4Controller.h"
#import "HS4.h"
#import "iHealthDeviceManagerModule.h"

#define EVENT_NOTIFY @"HS4.MODULE.NOTIFY" 

@implementation HS4SModule

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

#pragma mark-init

-(NSDictionary *)constantsToExport{
    return @{
             @"Event_Notify" :  EVENT_NOTIFY
             };
}


-(HS4 *)getHS4WithMac:(NSString *)mac{
    HS4Controller *controller = [HS4Controller shareIHHs4Controller];
    NSArray *hs4DeviceArray = [controller getAllCurrentHS4Instace];
    for (HS4 *tempHS4 in  hs4DeviceArray) {
        if ([mac isEqualToString:tempHS4.deviceID]){
            return tempHS4;
            break;
        }
    }
    return nil;
}

#pragma mark
#pragma mark - Notification
#pragma mark - HS4
-(void)DeviceConnectForHS4S:(NSNotification *)notify{
    HS4Controller *controller = [HS4Controller shareIHHs4Controller];
    NSArray *hs4DeviceArray = [controller getAllCurrentHS4Instace];
    
    HS4 *hs4Instance = [hs4DeviceArray objectAtIndex:0];
}

#pragma mark
#pragma mark - Method

RCT_EXPORT_METHOD(getOfflineData:(nonnull NSString*)mac){
    if ([self getHS4WithMac:mac] != nil) {
        HealthUser* healthUser = [[HealthUser alloc] init];
        healthUser.userID = [iHealthDeviceManagerModule autherizedUserID];
        healthUser.clientID = [iHealthDeviceManagerModule autherizedClientID];
        healthUser.clientSecret = [iHealthDeviceManagerModule autherizedClientSecret];
        [[self getHS4WithMac:mac]commandTransferMemorryWithUser:healthUser memoryData:^(NSDictionary *startDataDictionary) {
            NSDictionary *deviceInfo = @{@"mac":mac,@"action":@"Action_DataDictionary_HS",@"dictionary":[NSDictionary dictionaryWithObjectsAndKeys:startDataDictionary, nil]};
            
             [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
            
        } DisposeProgress:^(NSNumber *progress) {
            NSLog(@"progress:d%",progress);
            NSDictionary *deviceInfo = @{@"mac":mac,@"action":@"Action_Tansport_Porgress_HS",@"Progress":[NSNumber numberWithInteger:progress]};
            
             [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
            
        } MemorryData:^(NSArray *historyDataArray) {
            NSDictionary *deviceInfo = @{@"mac":mac,@"action":@"ACTION_HISTORICAL_DATA_HS",@"HISTORYDATA__HS":[NSArray arrayWithObjects:historyDataArray, nil]};
            
             [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
            
        } FinishTransmission:^{
            NSLog(@"FinishTransmission");
            NSDictionary *deviceInfo = @{@"mac":mac};
            
             [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
            
        } DisposeErrorBlock:^(HS4DeviceError errorID) {
            NSDictionary *deviceInfo = @{@"mac":mac,@"action":@"ACTION_ERROR_HS",@"error":[NSNumber numberWithInteger:errorID]};
            
             [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
        }];
    }
}


RCT_EXPORT_METHOD(measureOnline:(nonnull NSString*)mac :(nonnull NSNumber*)unit :(nonnull NSNumber*)userId){
    if ([self getHS4WithMac:mac] != nil) {
        HealthUser* healthUser = [[HealthUser alloc] init];
        healthUser.userID = [iHealthDeviceManagerModule autherizedUserID];
        healthUser.clientID = [iHealthDeviceManagerModule autherizedClientID];
        healthUser.clientSecret = [iHealthDeviceManagerModule autherizedClientSecret];
        [[self getHS4WithMac:mac]commandMeasureWithUint:HSUnit_Kg andUser:healthUser Authentication:^(UserAuthenResult result) {
            NSLog(@"UserAuthenResult:%d",result);
            if (result != UserAuthen_LoginSuccess) {
                [self sendErrorWithCode:result];
            }
            
        } Weight:^(NSNumber *unStableWeight) {
            NSDictionary *deviceInfo = @{@"mac":mac,@"action":ACTION_LIVEDATA_HS,LIVEDATA_HS:[NSNumber numberWithInteger:unStableWeight]};
            [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
            
        } StableWeight:^(NSDictionary *StableWeightDic) {
            NSDictionary *deviceInfo = @{@"mac":mac,@"action":ACTION_ONLINE_RESULT_HS,WEIGHT_HS:[NSDictionary dictionaryWithObjectsAndKeys:StableWeightDic, nil]};
            
            [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
            
        } DisposeErrorBlock:^(HS4DeviceError errorID) {
            NSDictionary *deviceInfo = @{@"mac":mac,@"action":ACTION_ERROR_HS,ERROR_NUM_HS:[NSNumber numberWithInteger:errorID]};
            [self.bridge.eventDispatcher sendDeviceEventWithName:EVENT_NOTIFY body:deviceInfo];
        }];
    }

}

RCT_EXPORT_METHOD(disconnect:(nonnull NSString*)mac){
    if ([self getHS4WithMac:mac] != nil){
        [[self getHS4WithMac:mac] commandDisconnectDevice];
        NSLog(@"End device connnect!");
    }
}

- (void)sendErrorWithCode:(NSInteger)errorCode{
    [self sendEventWithAction:@"ACTION_ERROR_HS" keyString:@"value" valueString:@(errorCode)];
}

- (void)sendEventWithAction:(NSString*)actionName keyString:(NSString*)key valueString:(id)value{
    [self.bridge.eventDispatcher sendDeviceEventWithName:@"HS4.MODULE.NOTIFY"  body:@{@"action":actionName,key:value}];
}

@end
