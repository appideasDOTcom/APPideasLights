//
//  AiUtil.m
//  AiDB
//
//  Created by Christopher Ostmo on 12/23/13.
//  Copyright (c) 2013 APPideas. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import <Photos/Photos.h>
#import <sys/utsname.h>

#include <ifaddrs.h>
#include <arpa/inet.h>

#import "AiUtil.h"
#import "constants.h"

@implementation AiUtil

@synthesize arbitraryNumber, webView;

-(id)init
{
	self = [super init];
	return self;
}

/**
 * Strip characters from a string
 *
 * <pre>NSMutableArray *remove = [NSMutableArray arrayWithCapacity:2];
 * [remove insertObject:@";" atIndex:0]; // remove semicolons
 * [remove insertObject:@"^" atIndex:1]; // remove carets
 * NSString "resultString = [AiUtil stripChars:remove fromString:inputString];</pre>
 */
+(NSString *)stripChars:(NSMutableArray *)charArray fromString:(NSString *)inputString
{
    NSString *returnValue = inputString;
    
    for( int i = 0; i < [charArray count]; i++ )
    {
        returnValue = [returnValue stringByReplacingOccurrencesOfString:[charArray objectAtIndex:i] withString:@""];
    }
    
    return returnValue;
}

/**
 * Convert an epoch date to something for humans.
 *
 * <pre>NSString showDate = [AiUtil readableDateFromEpoch:epochDate withFormat:_shortDateTimeFormat];</pre>
 */
+(NSString *)readableDateFromEpoch:(NSInteger)epochDate withFormat:(NSString *)outputFormat
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:outputFormat];
    NSDate *showDate = [NSDate dateWithTimeIntervalSince1970:epochDate];
    
    return [dateFormatter stringFromDate:showDate];
}

/**
 * Convenience wrapper to NSLog to honor the app _debug flag
 *
 * This can be used with the same argument structure as NSLog:
 * [AiUtil debugString:@"some string"]; or
 * [AiUtil debugString:@"some string %@ %@ %@", with, format, parmaters];
 *
 * @since		20140323
 * @author		costmo
 * @param       message             The string to print to the console
 * @param       format              Formatting parameters that match [NSString stringWithFormat:]
 */
+(void)debugString:(NSString *)message, ...
{
    if( _debug )
    {
        NSMutableString *newContentString = [NSMutableString string];
        [newContentString appendString:message];
        va_list args;
        va_start( args, message );
        NSLogv( message, args );
        va_end(args);
		
		// write debug messages to a file that we can get from iTunes
		if( _shareDebug )
		{
			va_list args;
			va_start( args, message );
			NSString *debugMessage = [[NSString alloc] initWithFormat:message arguments:args];
			va_end( args );
			
			NSDate *now = [NSDate date];
			NSDateFormatter *format = [[NSDateFormatter alloc] init];
			[format setDateFormat:@"yyyy-mm-dd HH:mm:ss zzz"];
			NSString *dateNow = [format stringFromDate:now];
			
			NSURL *applicationDocumentsDirectory = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory
																						   inDomains:NSUserDomainMask] lastObject];
			NSString *path = [applicationDocumentsDirectory.path
							  stringByAppendingString:@"/debug.log"];
			
			NSFileManager *fileManager = [NSFileManager defaultManager];
			BOOL isDir;
			if( ![fileManager fileExistsAtPath:path isDirectory:&isDir] )
			{
				[fileManager createFileAtPath:path contents:nil attributes:nil];
			}
			
			NSFileHandle *writerHandle = [NSFileHandle fileHandleForWritingAtPath:path];
			[writerHandle seekToEndOfFile];
			NSString *outputString = [NSString stringWithFormat:@"[%@] %@\n", dateNow, debugMessage];
			[writerHandle writeData:[outputString dataUsingEncoding:NSUTF8StringEncoding]];
			[writerHandle closeFile];
		} // if( _shareDebug )
    }
}

/**
 * Whether the app is running on a tablet (YES) or phone (NO)
 *
 * @since		20140323
 * @author		costmo
 */
+(BOOL)isTablet
{
    return (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
}

/**
 * Convenience method to make code easier to read. Returns the opposite of isTablet
 *
 * @since		20140323
 * @author		costmo
 */
+(BOOL)isPhone
{
	return ![AiUtil isTablet];
}

/**
 * Strip whitespace from the beginning and end of a string
 *
 * @since		20140323
 * @author		costmo
 */
+(NSString *)trimString:(NSString *)input;
{
    return [input stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
}

/**
 * Gets the MD5 hash of a string
 *
 * @return		MD5 hash of a string
 * @since		20140323
 * @author		costmo
 * @param		input	The input hash
 */
+(NSString *)md5HashOfString:(NSString *)input
{
	const char *cStr = [input UTF8String];
	unsigned char result[16];
	CC_MD5( cStr, (CC_LONG)strlen( cStr ), result );
	NSString *returnString = [NSString stringWithFormat:
							  @"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
							  result[0], result[1], result[2], result[3],
							  result[4], result[5], result[6], result[7],
							  result[8], result[9], result[10], result[11],
							  result[12], result[13], result[14], result[15]
							  ];
	return [returnString lowercaseString];
}

+(NSInteger)currentTimestamp
{
	return [[NSDate date] timeIntervalSince1970];
}

+(NSString *)elapsedFromSeconds:(int)input
{
	NSInteger eval = input;
	if( eval <=  -100 || input >= (INT_MAX -  10))
	{
		input = 0;
	}

	int seconds = input % 60;
	int minutes = (input / 60) % 60;
	int hours = input / 3600;
	
	return [NSString stringWithFormat:@"%01d:%02d:%02d", hours, minutes, seconds];

}


/**
 * Make sure the needed view tag and storyboard ID arrays are available
 *
 * @returns		UIColor
 * @since		20141008
 * @author		costmo
 * @param		hexValue			The hex value to use for the color. e.g. ff000 for "red"
 */
+(UIColor *)UIColorFromRGB:(NSString *)hexValue
{
	unsigned rgbValue = 0;
	NSScanner *scanner = [NSScanner scannerWithString:hexValue];
	[scanner scanHexInt:&rgbValue];
	
	return [UIColor
			colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0
			green:((float)((rgbValue & 0xFF00) >> 8))/255.0
			blue:((float)(rgbValue & 0xFF))/255.0 alpha:1.0];
}



+(NSString *)nl2p:(NSString *)inputString
{
	NSArray *paragraphs = [inputString componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
	
	NSString *outputContent = @"";
	for( NSInteger i = 0; i < [paragraphs count]; i++ )
	{
		outputContent = [outputContent stringByAppendingFormat:@"<p>%@</p>", [paragraphs objectAtIndex:i]];
	}
	
	return outputContent;
}


+(NSString *)nl2br:(NSString *)inputString
{
	NSArray *lines = [inputString componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
	
	NSString *outputContent = @"";
	for( NSInteger i = 0; i < [lines count]; i++ )
	{
		outputContent = [outputContent stringByAppendingFormat:@"%@<br/>", [lines objectAtIndex:i]];
	}
	
	return outputContent;
}

+(NSString *)stringFromEpoch:(NSInteger)epoch withFormat:(NSString *)dateFormat
{
	NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
	[dateFormatter setTimeZone:[NSTimeZone timeZoneWithAbbreviation:@"GMT"]];
	[dateFormatter setDateFormat:dateFormat];
	NSDate *dateValue = [[NSDate alloc] initWithTimeIntervalSince1970:epoch];
	NSString *returnValue = [dateFormatter stringFromDate:dateValue];
	
	return returnValue;
}

+(NSString *)stringFromLocalTimeEpoch:(NSInteger)epoch withFormat:(NSString *)dateFormat
{
	NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
	[dateFormatter setDateFormat:dateFormat];
	NSDate *dateValue = [[NSDate alloc] initWithTimeIntervalSince1970:epoch];
	NSString *returnValue = [dateFormatter stringFromDate:dateValue];
	
	return returnValue;
}

+ (NSString *)stripTags:(NSString *)str
{
	NSMutableString *html = [NSMutableString stringWithCapacity:[str length]];
	
	NSScanner *scanner = [NSScanner scannerWithString:str];
	scanner.charactersToBeSkipped = NULL;
	NSString *tempText = nil;
	
	while (![scanner isAtEnd])
	{
		[scanner scanUpToString:@"<" intoString:&tempText];
		
		if (tempText != nil)
			[html appendString:tempText];
		
		[scanner scanUpToString:@">" intoString:NULL];
		
		if (![scanner isAtEnd])
			[scanner setScanLocation:[scanner scanLocation] + 1];
		
		tempText = nil;
	}
	
	// Convert our mutable string to NSString
	NSString *returnString = html;
	
	returnString = [returnString stringByReplacingOccurrencesOfString:@"&nbsp;" withString:@" "];
	
	return returnString;
}

+(BOOL)emailIsValid:(NSString *)input
{
	BOOL returnValue = YES;
	
	if( [input length] < 6 )
	{
		return NO;
	}
	
	NSArray *tokens = [input componentsSeparatedByString:@"@"];
	if( [tokens count] != 2 )
	{
		return NO;
	}
	
	NSArray *subTokens = [tokens[1] componentsSeparatedByString:@"."];
	if( [subTokens count] < 2 )
	{
		return NO;
	}
	
	if( [subTokens[0] length] < 1 || [subTokens[1] length] < 2 )
	{
		return NO;
	}
	
	return returnValue;
}

+(NSString *)splitDomainFromEmail:(NSString *)input
{
	NSString *returnValue = @"";
	
	NSArray *tokens = [input componentsSeparatedByString:@"@"];
	if( [tokens count] != 2 )
	{
		return returnValue;
	}
	
	return [tokens objectAtIndex:1];
}

+(void)setConstraint:(NSString *)which forView:(UIView *)view toValue:(float)value
{
	NSLayoutConstraint *constraint = nil;
	
	// top and bottom are against the superview of the target
	for( NSLayoutConstraint *currentConstraint in view.superview.constraints )
	{
		if( [which compare:@"top"] == NSOrderedSame || [which compare:@"bottom"] == NSOrderedSame || [which compare:@"centery"] == NSOrderedSame )
		{
			int layoutAttribute = NSLayoutAttributeTop;
			
			if( [which compare:@"bottom"] == NSOrderedSame)
			{
				layoutAttribute = NSLayoutAttributeBottom;
			}
			else if( [which compare:@"centery"] == NSOrderedSame)
			{
				layoutAttribute = NSLayoutAttributeCenterY;
			}
			
			if( (currentConstraint.firstItem == view && currentConstraint.firstAttribute == layoutAttribute) ||
			   (currentConstraint.secondItem == view && currentConstraint.secondAttribute == layoutAttribute) )
			{
				constraint = currentConstraint;
			}
		}
	}
	
	// either the view wasn't found, or we're dealing with width/height
	if( constraint == nil )
	{
		for( NSLayoutConstraint *currentConstraint in view.constraints )
		{
			// height and width are against the target view
			if( [which compare:@"height"] == NSOrderedSame || [which compare:@"width"] == NSOrderedSame )
			{
				int layoutAttribute = ([which compare:@"height"] == NSOrderedSame) ? NSLayoutAttributeHeight : NSLayoutAttributeWidth;
				
				if( (currentConstraint.firstItem == view && currentConstraint.firstAttribute == layoutAttribute) ||
			    (currentConstraint.secondItem == view && currentConstraint.secondAttribute == layoutAttribute) )
				{
					constraint = currentConstraint;
				}
			}
		}
	}
	
	if( constraint != nil )
	{
		constraint.constant = value;
	}
}

// aspect ratio is W/H
+(void)setAspectRatio:(float)ratio forView:(UIView *)view
{
	NSLayoutConstraint *heightConstraint = nil;
	NSLayoutConstraint *widthConstraint = nil;
	
	for( NSLayoutConstraint *currentConstraint in view.constraints )
	{
		if( (currentConstraint.firstItem == view && currentConstraint.firstAttribute == NSLayoutAttributeHeight) ||
		(currentConstraint.secondItem == view && currentConstraint.secondAttribute == NSLayoutAttributeHeight) )
		{
			heightConstraint = currentConstraint;
		}
		
		if( (currentConstraint.firstItem == view && currentConstraint.firstAttribute == NSLayoutAttributeWidth) ||
		   (currentConstraint.secondItem == view && currentConstraint.secondAttribute == NSLayoutAttributeWidth) )
		{
			widthConstraint = currentConstraint;
		}

	}
	
	if( heightConstraint != nil && widthConstraint != nil )
	{
		[view removeConstraint:widthConstraint];
		[view removeConstraint:heightConstraint];
		
		[view addConstraint:
			[NSLayoutConstraint
			 constraintWithItem:view
			 attribute:NSLayoutAttributeWidth
			 relatedBy:NSLayoutRelationEqual
			 toItem:view
			 attribute:NSLayoutAttributeHeight
			 multiplier:ratio
			 constant:0.0]
		 ];
	}
}

+(void)showError:(NSString *)message withTitle:(NSString *)title fromCaller:(id)caller
{
	dispatch_async(dispatch_get_main_queue(),
				   ^{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
													message:message
												   delegate:caller
										  cancelButtonTitle:NSLocalizedString( @"ok", nil )
										  otherButtonTitles:nil];
					   [alert show];
				   });
	
}

+(NSString *)md5HashOfPath:(NSString *)path
{
	NSFileManager *fileManager = [NSFileManager defaultManager];
	// Make sure the file exists
	if( [fileManager fileExistsAtPath:path isDirectory:nil] )
	{
		NSData *data = [NSData dataWithContentsOfFile:path];
		unsigned char digest[CC_MD5_DIGEST_LENGTH];
		CC_MD5( data.bytes, (CC_LONG)data.length, digest );
		
		NSMutableString *output = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
		
		for( int i = 0; i < CC_MD5_DIGEST_LENGTH; i++ )
		{
			[output appendFormat:@"%02x", digest[i]];
		}
		
		return output;
	}
	else
	{
		return @"";
	}
}

+(NSString *)shaHashOfPath:(NSString *)path
{
	NSFileManager *fileManager = [NSFileManager defaultManager];
	// Make sure the file exists
	if( [fileManager fileExistsAtPath:path isDirectory:nil] )
	{
		NSData *data = [NSData dataWithContentsOfFile:path];
		unsigned char digest[CC_SHA1_DIGEST_LENGTH];
		CC_SHA1( data.bytes, (CC_LONG)data.length, digest );
		
		NSMutableString *output = [NSMutableString stringWithCapacity:CC_SHA1_DIGEST_LENGTH * 2];
		
		for( int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++ )
		{
			[output appendFormat:@"%02x", digest[i]];
		}
		
		return output;
	}
	else
	{
		return @"";
	}
}

+(UIImage *)fixRotation:(UIImage *)image
{
	if (image.imageOrientation == UIImageOrientationUp) return image;
	CGAffineTransform transform = CGAffineTransformIdentity;
	
	switch (image.imageOrientation) {
		case UIImageOrientationDown:
		case UIImageOrientationDownMirrored:
			transform = CGAffineTransformTranslate(transform, image.size.width, image.size.height);
			transform = CGAffineTransformRotate(transform, M_PI);
			break;
			
		case UIImageOrientationLeft:
		case UIImageOrientationLeftMirrored:
			transform = CGAffineTransformTranslate(transform, image.size.width, 0);
			transform = CGAffineTransformRotate(transform, M_PI_2);
			break;
			
		case UIImageOrientationRight:
		case UIImageOrientationRightMirrored:
			transform = CGAffineTransformTranslate(transform, 0, image.size.height);
			transform = CGAffineTransformRotate(transform, -M_PI_2);
			break;
		case UIImageOrientationUp:
		case UIImageOrientationUpMirrored:
			break;
	}
	
	switch (image.imageOrientation) {
		case UIImageOrientationUpMirrored:
		case UIImageOrientationDownMirrored:
			transform = CGAffineTransformTranslate(transform, image.size.width, 0);
			transform = CGAffineTransformScale(transform, -1, 1);
			break;
			
		case UIImageOrientationLeftMirrored:
		case UIImageOrientationRightMirrored:
			transform = CGAffineTransformTranslate(transform, image.size.height, 0);
			transform = CGAffineTransformScale(transform, -1, 1);
			break;
		case UIImageOrientationUp:
		case UIImageOrientationDown:
		case UIImageOrientationLeft:
		case UIImageOrientationRight:
			break;
	}
	
	// Now we draw the underlying CGImage into a new context, applying the transform
	// calculated above.
	CGContextRef ctx = CGBitmapContextCreate(NULL, image.size.width, image.size.height,
											 CGImageGetBitsPerComponent(image.CGImage), 0,
											 CGImageGetColorSpace(image.CGImage),
											 CGImageGetBitmapInfo(image.CGImage));
	CGContextConcatCTM(ctx, transform);
	switch (image.imageOrientation) {
		case UIImageOrientationLeft:
		case UIImageOrientationLeftMirrored:
		case UIImageOrientationRight:
		case UIImageOrientationRightMirrored:
			// Grr...
			CGContextDrawImage(ctx, CGRectMake(0,0,image.size.height,image.size.width), image.CGImage);
			break;
			
		default:
			CGContextDrawImage(ctx, CGRectMake(0,0,image.size.width,image.size.height), image.CGImage);
			break;
	}
	
	// And now we just create a new UIImage from the drawing context
	CGImageRef cgimg = CGBitmapContextCreateImage(ctx);
	UIImage *img = [UIImage imageWithCGImage:cgimg];
	CGContextRelease(ctx);
	CGImageRelease(cgimg);
	return img;
}

+(NSString *)mimeTypeForPath:(NSString *)path
{
	// get a mime type for an extension using MobileCoreServices.framework
	
	CFStringRef extension = (__bridge CFStringRef)[path pathExtension];
	CFStringRef UTI = UTTypeCreatePreferredIdentifierForTag( kUTTagClassFilenameExtension, extension, NULL );
	assert(UTI != NULL);
	
	NSString *mimetype = CFBridgingRelease( UTTypeCopyPreferredTagWithClass( UTI, kUTTagClassMIMEType ) );
	assert( mimetype != NULL );
	
	CFRelease( UTI );
	
	return mimetype;
}

+(NSString *)generateBoundaryString
{
	return [NSString stringWithFormat:@"Boundary-%@", [[NSUUID UUID] UUIDString]];
}

+(NSData *)createPostBodyWithBoundary:(NSString *)boundary
						parameters:(NSDictionary *)parameters
							 paths:(NSArray *)paths
						 fieldName:(NSString *)fieldName
{
	NSMutableData *httpBody = [NSMutableData data];
	
	// add params (all params are strings)
	
	[parameters enumerateKeysAndObjectsUsingBlock:^(NSString *parameterKey, NSString *parameterValue, BOOL *stop) {
		[httpBody appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
		[httpBody appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"\r\n\r\n", parameterKey] dataUsingEncoding:NSUTF8StringEncoding]];
		[httpBody appendData:[[NSString stringWithFormat:@"%@\r\n", parameterValue] dataUsingEncoding:NSUTF8StringEncoding]];
	}];
	
	// add image data
	
	for (NSString *path in paths) {
		NSString *filename  = [path lastPathComponent];
		NSData   *data      = [NSData dataWithContentsOfFile:path];
		NSString *mimetype  = [self mimeTypeForPath:path];
		
		[httpBody appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
		[httpBody appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"; filename=\"%@\"\r\n", fieldName, filename] dataUsingEncoding:NSUTF8StringEncoding]];
		[httpBody appendData:[[NSString stringWithFormat:@"Content-Type: %@\r\n\r\n", mimetype] dataUsingEncoding:NSUTF8StringEncoding]];
		[httpBody appendData:data];
		[httpBody appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
	}
	
	[httpBody appendData:[[NSString stringWithFormat:@"--%@--\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
	
	return httpBody;
}

+(UIImage *)overlayImage:(UIImage *)image withColor:(UIColor *)color
{
	//  Create rect to fit the PNG image
	CGRect rect = CGRectMake(0, 0, image.size.width, image.size.height);
	
	//  Start drawing
	UIGraphicsBeginImageContext(rect.size);
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSaveGState(context);
	
	//  Fill the rect by the final color
	[color setFill];
	CGContextFillRect(context, rect);
	
	//  Make the final shape by masking the drawn color with the images alpha values
	CGContextSetBlendMode(context, kCGBlendModeDestinationIn);
	[image drawInRect: rect blendMode:kCGBlendModeDestinationIn alpha:1];
	
	//  Create new image from the context
	UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
	
	//  Release context
	UIGraphicsEndImageContext();
	
	return img;
}
/**
 * Get the HTML header and footer for surrounding the actual content
 *
 * @returns		A dictionary with 'head' and 'tail' keys
 * @since		20140324
 * @author		costmo
 */
+(NSMutableDictionary *)getHtmlEnvelope
{
	NSMutableDictionary *returnValue = [[NSMutableDictionary alloc] initWithCapacity:2];
	
	NSString *headerString =
	  @" \
	  <html> \
		<head> \
			<style> \
				body{ background:#fff;font-family:'SF UI Display',Helvetica,sans-serif;color:#000;margin-left:10px;margin-right:10px;font-size:13; } \
				.agreement { margin-top:20px; font-size: 10; } \
			</style> \
		</head> \
		<body height='100%'> \
			<div style='width:100%;'> \
	  ";
	
	[returnValue setObject:headerString forKey:@"head"];
	
	[returnValue
	 setObject:@" \
			</div>\
		</body>\
	 </html>"
	 forKey:@"tail"];
	
	return returnValue;
}

+(void)rotateToDeviceOrientation
{
	// no phones!
	if( ![AiUtil isTablet] )
	{
		return;
	}
	
	[UIViewController attemptRotationToDeviceOrientation];
}

+(BOOL)isCameraAvailable
{
	BOOL returnValue = YES;
	
	// Image picker will return YES if the camera is disabled for the app, but NO if it is disabled universally
	UIImagePickerController *picker = [[UIImagePickerController alloc] init];
	picker.mediaTypes = [[NSArray alloc] initWithObjects: (NSString *) kUTTypeImage, nil];
	if( ![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera] )
	{
		returnValue = NO;
	}
	
	if( returnValue == YES )
	{
		AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
		if( status == AVAuthorizationStatusAuthorized )
		{
			returnValue = YES;
		}
		else if( status == AVAuthorizationStatusDenied )
		{
			returnValue = NO;
		}
		else if( status == AVAuthorizationStatusRestricted )
		{
			returnValue = NO;
		}
		else if( status == AVAuthorizationStatusNotDetermined )
		{
			__block BOOL returnable;
			[AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:
			 ^(BOOL granted)
			{
				if( granted )
				{
					returnable = YES;
				}
				else
				{
					returnable = NO;
				}
			}];
			
			returnValue = returnable;
		}
	}
	
	return returnValue;
}

+(BOOL)isPhotoLibraryAvailable
{
	BOOL returnValue = YES;
	
	if( ![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary] )
	{
		returnValue = NO;
	}
	
	if(returnValue == YES )
	{
		PHAuthorizationStatus status = [PHPhotoLibrary authorizationStatus];
		if( status == PHAuthorizationStatusAuthorized )
		{
			returnValue = YES;
		}
		else if( status == PHAuthorizationStatusDenied )
		{
			returnValue = NO;
		}
		else if( status == PHAuthorizationStatusRestricted )
		{
			returnValue = NO;
		}
		else if( status == PHAuthorizationStatusNotDetermined )
		{
			__block BOOL returnable;
			[PHPhotoLibrary requestAuthorization:
			 ^(PHAuthorizationStatus authorizationStatus)
			 {
				 if( authorizationStatus == PHAuthorizationStatusAuthorized )
				 {
					 returnable = YES;
				 }
				 else
				 {
					 returnable = NO;
				 }
			 }];
			
			returnValue = returnable;
		}
	}
	
	return returnValue;
}



+(NSString*) deviceName
{
	struct utsname systemInfo;
	uname(&systemInfo);
	
	return [NSString stringWithCString:systemInfo.machine
							  encoding:NSUTF8StringEncoding];
}

+ (NSString *)getIPAddress {
    
    NSString *address = @"error";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"]) {
                    // Get NSString from C String
                    address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];
                    
                }
                
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return address;
    
} 


@end
