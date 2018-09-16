//
//  MainViewController.m
//  AppideasLights
//
//  Created by Chris Ostmo on 9/14/18.
//  Copyright © 2018 Chris Ostmo. All rights reserved.
//

#import "MainViewController.h"
#import "constants.h"
#import "ALApiController.h"
#import "AiUtil.h"

@interface MainViewController ()

@end

@implementation MainViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    if( [self needSetup] )
    {
        [AiUtil debugString:@"need setup"];
        dispatch_async(dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 ),
       ^{
           [self performSegueWithIdentifier:@"showAPSetup" sender:nil];
       });
    }
}

-(BOOL)needSetup
{
    bool returnValue = true;

    NSString *ip = [AiUtil getIPAddress];
    [AiUtil debugString:@"IP: %@", ip];
    
    if( ![ip hasPrefix:@"192.168.4."] )
    {
        returnValue = false;
    }
    
    return returnValue;
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
