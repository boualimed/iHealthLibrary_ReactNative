//
//  BP5ViewController.h
//  testShareCommunication
//
//  Created by my on 14/10/13.
//  Copyright (c) 2013年 my. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BP5Controller : NSObject{
    NSMutableArray *BP5DeviceArray;
}

/**
 * Initialize BP5 controller class
 */
+(BP5Controller *)shareBP5Controller;

/**
 * Get all BP5 instance,Access control class instance after receiving BP5ConnectNoti, then use instance to call BP5 related communication methods.
 */
-(NSArray *)getAllCurrentBP5Instace;
@end
