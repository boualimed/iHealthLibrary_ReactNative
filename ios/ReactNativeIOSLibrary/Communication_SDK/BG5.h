//
//  BG5.h
//  testShareCommunication
//
//  Created by daiqingquan on 14-1-16.
//  Copyright (c) 2014年 my. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BGMacroFile.h"

//
typedef enum{
    BG5CommError = 1
    
}BG5DeviceError;



@interface BG5 : NSObject{

    uint8_t allCodeBuf[170];
    uint8_t allCTLCodeBuf[170];
    NSNumber*bgunit;
    DisposeBGErrorBlock _disposeBGErrorBlock;
    DisposeBGSetTime _disposeBGSetTime;
    DisposeBGSetUnit _disposeBGSetUnit;
    DisposeBGBottleID _disposeBGBottleID;
    DisposeBGDataCount _disposeBGDataCount;
    DisposeBGHistoryData _disposeBGHistoryData;
    DisposeBGDeleteData _disposeBGDeleteData;
    DisposeBGStripInBlock _disposeBGStripInBlock;
    DisposeBGStripOutBlock _disposeBGStripOutBlock;
    DisposeBGBloodBlock _disposeBGBloodBlock;
    DisposeBGResultBlock _disposeBGResultBlock;
    DisposeBGCodeDic _disposeBGCodeDic;
    DisposeBGSendBottleIDBlock _disposeBGSendBottleIDBlock;
    DisposeBGSendCodeBlock _disposeBGSendCodeBlock;
    DisposeBGStartModel _disposeBGStartModel;
    DisposeBGTestModelBlock _disposeBGTestModelBlock;
    DisposeAuthenticationBlock _disposeAuthenticationBlock;
    DisposeBGBatteryBlock _disposeBG5BatteryBlock;
    DisposeBG5KeepConnectBlock _disposeBG5KeepConnectBlock;

    
    NSString *currentUserID;
    
    NSString *clientSDKUserName;
    NSString *clientSDKID;
    NSString *clientSDKSecret;
    
    BOOL modelVerifyOK;
    int memeryNum;
    BOOL supportQeryEnergyFlg;
    NSMutableArray *historyArray;
    int testTypeNum; //用于开始测量发送测量模式

}
@property (strong, nonatomic) NSString *currentUUID;
///‘serialNumber’ is for separating different device when multiple device have been connected.
@property (strong, nonatomic) NSString *serialNumber;
@property (strong, nonatomic) NSString *firmwareVersion;
@property(strong, nonatomic) NSNumber* reactNativeFlg;  //reactNative开关，YES时不走SDK认证等，NO走SDK所有流程。
/**
 * Establish measurement connection
 * @param userID  The only user label, is indicated by form of email address.
 * @param clientID 
 * @param clientSecret  'clientID' and 'clientSecret' are the only user label, will be achieved after the register of SDK application. Please contact louie@ihealthlabs.com for the registration.
 * @param unitstate   BGUnit_mmolPL stands for mmol/L, BGUnit_mgPmL stands for mg/dL.
 * @param disposeAuthenticationBlock This block returns results after  the verification of userID,clientID,clientSecret.
 * Results:
 *      a)	UserAuthen_RegisterSuccess, new register successes.
 *      b)	UserAuthen_LoginSuccess, user logs in successfully.
 *      c)	UserAuthen_CombinedSuccess, user has been recognised as iHealth user, the measurement via SDK could be activated, the result data belongs to the user.
 *      d)	UserAuthen_TrySuccess, network error, the measurement is only for testing, SDK is not fully functional.
 *      e)	UserAuthen_InvalidateUserInfo, the verification of userID/clientID/clientSecret failed.
 *      f)	UserAuthen_SDKInvalidateRight, the application has not been authorised.
 *      g)	UserAuthen_UserInvalidateRight, the user has not been authorised.
 *      h)	UserAuthen_InternetError, network error, verification    failed.
 *   -- PS:
 *      1. the measurement via SDK is functional in the case from a) to d).
 *      2. the measurement via SDK will be determined in the case from e) to h), please contact iHealth support team, louie@ihealthlabs.com
 *      3. “iHealth Disclaimer” will pop up and need to be proved by the user when SDK is activated for the first time.
 *      4. if iHealth SDK has been using without internet, there is only 10-day try out because the SDK can not be certified.
 * @param disposeBGBottleID  This block returns the ID which is stored in the BG meter, to verify if the strip has been used is from the same bottle of the registered one. if not, the app will notify the user need scan the new bottle, if yes, the app will get the number of left strips and expire date.
 * @param disposeBGErrorBlock   This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandInitBGSetUnit:(BGUnit )unitState BGUserID:(NSString*)userID clientID:(NSString *)clientID clientSecret:(NSString *)clientSecret Authentication:(DisposeAuthenticationBlock)disposeAuthenticationBlock DisposeBGBottleID:(DisposeBGBottleID)disposeBGBottleID DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;


/**
 * Set Time (only for react-Native project)
 * @param disposeBGSetTime The block return means set success.
 * @param disposeBGErrorBlock, This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandBGSetTime:(DisposeBGSetTime)disposeBGSetTime DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;

/**
 * Set Unit (only for react-Native project)
 * @param disposeBGSetUnit The block return means set success.
 * @param disposeBGErrorBlock, This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandBGSetUnit:(BGUnit )unitState DisposeSetUnitResult:(DisposeBGSetUnit)disposeBGSetUnitResult DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;

/**
 * Get the bottleID stored in the BG meter (only for react-Native project)
 * @param disposeBGBottleID  This block returns the ID which is stored in the BG meter, to verify if the strip has been used is from the same bottle of the registered one. if not, the app will notify the user need scan the new bottle, if yes, the app will get the number of left strips and expire date.
 * @param disposeBGErrorBlock, This block returns error codes,please refer to error codes list in BGMacroFile.
 */

-(void)commandBGGetBottleID:(DisposeBGBottleID)disposeBGBottleID DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;

/**
 * Tranfer offline history records
 * @param disposeBGDataCount   The number of the records in the meter memory.
 * @param disposeBGHistoryData  The offline history records detail, result means result, date means the measurement time.
 * @param disposeBGErrorBlock  This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandTransferMemorryData:(DisposeBGDataCount)disposeBGDataCount DisposeBGHistoryData:(DisposeBGHistoryData)disposeBGHistoryData DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;


/**
 * Delete offline history records
 * @param DisposeBGDeleteData This block returns yes means deleted success.
 */
-(void)commandDeleteMemorryData:(DisposeBGDeleteData)DisposeBGDeleteData;


/**
 * Read the information of the code from the BG meter
 * @param  disposeBGCodeDic  This block returns the information of the code, Strips means the number of strips which has been used, Date means expired date.
 * @param  disposeBGErrorBlock  This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandReadBGCodeDic:(DisposeBGCodeDic)disposeBGCodeDic DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;

/**
 * Send code with type (New)
 * @param testType Set the measure test type,@1 is Blood Test,@2 is CTL Test.
 * @param codeType Set the code type,@1 is GOD,@2 is GDH.
 * @param encodeString  The code String gets by scanning the QR code.
 * @param bottleID   Generate from the QR code through some algorithms,ranging from 0-0xFFFFFFFF.
 * @param date  The expired date.
 * @param num  the number of strips which is last,ranging from 0-255. num = 0 will determine the measurement.
 * @param disposeBGSendCodeBlock  This block returns yes means code has been sent regularly.
 * @param disposeBGStartModel  The boot mode of the BG meter, BGOpenMode_Strip means booting the meter by sliding the strip, BGOpenMode_Hand means booting the meter by pressing the on/off button. interface (6) will be called by the first mode, interface (7) will be called by the second mode.
 * @param disposeBGErrorBlock This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandSendBGCodeWithMeasureType:(BGMeasureMode)testType CodeType:(BGCodeMode)codeType CodeString:(NSString*)encodeString bottleID:(NSNumber *)bottleID validDate:(NSDate*)date remainNum:(NSNumber*)num DisposeBGSendCodeBlock:(DisposeBGSendCodeBlock)disposeBGSendCodeBlock DisposeBGStartModel:(DisposeBGStartModel)disposeBGStartModel DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;


/**
 * Send code (Deprecated since 2016/12/15,-- use 'Send code with type'function instead.)
 * @param encodeString  The code String gets by scanning the QR code.
 * @param bottleID   Generate from the QR code through some algorithms,ranging from 0-0xFFFFFFFF.
 * @param date  The expired date.
 * @param num  the number of strips which is last,ranging from 0-255. num = 0 will determine the measurement.
 * @param disposeBGSendCodeBlock  This block returns yes means code has been sent regularly.
 * @param disposeBGStartModel  The boot mode of the BG meter, BGOpenMode_Strip means booting the meter by sliding the strip, BGOpenMode_Hand means booting the meter by pressing the on/off button. interface (6) will be called by the first mode, interface (7) will be called by the second mode.
 * @param disposeBGErrorBlock  This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandSendBGCodeString:(NSString*)encodeString bottleID:(NSNumber *)bottleID validDate:(NSDate*)date remainNum:(NSNumber*)num DisposeBGSendCodeBlock:(DisposeBGSendCodeBlock)disposeBGSendCodeBlock DisposeBGStartModel:(DisposeBGStartModel)disposeBGStartModel DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;


/**
 * Strip-Sliding booting mode
 * @param disposeBGStripInBlock This block returns yes means strip slides in.
 * @param disposeBGBloodBlock This block returns yes means the blood drop has beed sensed from the strip.
 * @param disposeBGResultBlock This block returns the measurement by the unit of mg/dL, range from 20-600.
 * @param disposeBGTestModelBlock This block returns measurement mode, BGMeasureMode_Blood means blood measurement mode, BGMeasureMode_NoBlood means control solution measurement mode.
 * @param disposeBGErrorBlock   This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandCreateBGtestStripInBlock:(DisposeBGStripInBlock)disposeBGStripInBlock DisposeBGBloodBlock:(DisposeBGBloodBlock)disposeBGBloodBlock DisposeBGResultBlock:(DisposeBGResultBlock)disposeBGResultBlock  DisposeBGTestModelBlock:(DisposeBGTestModelBlock)disposeBGTestModelBlock DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;


/**
 * Button-pressing booting mode
 * @param testMode  Set the measurement mode,must be the same as the testType in send code method, BGMeasureMode_Blood means blood measurement mode, BGMeasureMode_NoBlood means control solution measurement mode.
 * @param disposeBGStripInBlock  This block returns yes means strip slides in.
 * @param disposeBGBloodBlock This block returns yes means the blood drop has beed sensed from the strip.
 * @param disposeBGResultBlock  This block returns the measurement by the unit of mg/dL, range from 20-600.
 * @param disposeBGTestModelBlock  This block returns measurement mode, BGMeasureMode_Blood means blood measurement mode, BGMeasureMode_NoBlood means control solution measurement mode.
 * @param disposeBGErrorBlock   This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandCreateBGtestModel:(BGMeasureMode)testMode DisposeBGStripInBlock:(DisposeBGStripInBlock)disposeBGStripInBlock DisposeBGBloodBlock:(DisposeBGBloodBlock)disposeBGBloodBlock DisposeBGResultBlock:(DisposeBGResultBlock)disposeBGResultBlock DisposeBGTestModelBlock:(DisposeBGTestModelBlock)disposeBGTestModelBlock  DisposeBGErrorBlock:(DisposeBGErrorBlock)disposeBGErrorBlock;

/**
 * Analyze code include bottleID，DueDate and the number of strips.
 * @param encodeString  The code String gets by scanning the QR code.
 */
-(NSDictionary *)codeStripStrAnalysis:(NSString *)encodeString;


/**
 * Query battery remaining energy
 * @param disposeBatteryBlock  A block to return the device battery remaining energy percentage, ‘80’ stands for 80%.
 * @param disposeErrorBlock   This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandQueryBattery:(DisposeBGBatteryBlock)disposeBatteryBlock DisposeErrorBlock:(DisposeBGErrorBlock)disposeErrorBlock;

/**
 * Keep the connection
 * Keep the connection to wait for user to scan code,need to loop call.
 * @param disposeBG5KeepConnectBlock A block returns the result of the keeping connection order,'YES' means setting success.
 * @param disposeErrorBlock   This block returns error codes,please refer to error codes list in BGMacroFile.
 */
-(void)commandKeepConnect:(DisposeBG5KeepConnectBlock)disposeBG5KeepConnectBlock DisposeErrorBlock:(DisposeBGErrorBlock)disposeErrorBlock;


@end
