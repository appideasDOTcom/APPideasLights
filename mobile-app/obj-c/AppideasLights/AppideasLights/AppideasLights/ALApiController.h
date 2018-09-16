//
//  ALApiController.h
//  AppideasLights
//
//  Created by Chris Ostmo on 9/14/18.
//  Copyright © 2018 Chris Ostmo. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ALApiController : NSObject <NSURLConnectionDataDelegate, NSURLConnectionDelegate>
{
    /**
     * Received downloaded data
     */
    NSMutableData *downloadData;
    
    /**
     * Total number of bytes to be downloaded
     */
    NSUInteger totalBytes;
    
    /**
     * Number of bytes received so far
     */
    NSUInteger receivedBytes;
    
    /**
     * Whether or not a download is currently in progress
     */
    BOOL isDownloading;
    
    /**
     * YES if there was a connection error
     */
    BOOL connectionDidError;
    
    /**
     * A place to store the error for a caller to retrieve
     */
    NSString *errorMessage;
    
    /**
     * YES for the API controller to show errors, NO for the caller to do so
     */
    BOOL displayErrors;
    
    NSInteger retryCount;
    
    NSInteger waitTimeout;
}

@property (nonatomic, retain) NSMutableData *downloadData;
@property (nonatomic, assign) NSUInteger totalBytes;
@property (nonatomic, assign) NSUInteger receivedBytes;
@property (nonatomic, assign) BOOL isDownloading;
@property (nonatomic, assign) BOOL connectionDidError;
@property (nonatomic, retain) NSString *errorMessage;
@property (nonatomic, assign) BOOL displayErrors;
@property (nonatomic, assign) NSInteger retryCount;

@property (nonatomic, assign) NSInteger waitTimeout;

-(NSMutableDictionary *)dataFromUrl:(NSString *)url;

@end
