//
//  ALApiController.m
//  AppideasLights
//
//  Created by Chris Ostmo on 9/14/18.
//  Copyright © 2018 Chris Ostmo. All rights reserved.
//

#import "ALApiController.h"
#import "AiUtil.h"

@implementation ALApiController

@synthesize downloadData, totalBytes, receivedBytes, isDownloading, connectionDidError, errorMessage, displayErrors, retryCount, waitTimeout;

-(id)init
{
    self = [super init];
    
    self.downloadData = nil;
    self.totalBytes = 0;
    self.receivedBytes = 0;
    self.isDownloading = NO;
    self.connectionDidError = NO;
    self.errorMessage = @"";
    self.displayErrors = YES;
    self.retryCount = 0;
    self.waitTimeout = 3;
    
    return self;
}

-(NSMutableDictionary *)dataFromUrl:(NSString *)url
{
    self.connectionDidError = NO;
    NSError *error = nil;
    NSURLResponse *response = nil;
    
    NSURL *getUrl = [NSURL URLWithString:url];
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:getUrl];
    request.HTTPMethod = @"GET";
    
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    
    if( error )
    {
        self.errorMessage = [NSString stringWithFormat:@"%@. %@", NSLocalizedString( @"couldNotConnect", nil ), error.localizedDescription];
        self.connectionDidError = YES;
        [AiUtil debugString:self.errorMessage];
        
        return [[NSMutableDictionary alloc] initWithCapacity:0];
    }
    
    NSMutableDictionary *gotValues = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    
    if( gotValues == nil )
    {
        //[AppDelegate trackEvent:@"nil_data" forCategory:@"api_controller"];
        return [[NSMutableDictionary alloc] initWithCapacity:0];
    }
    else
    {
        return gotValues;
    }
    
}

@end
