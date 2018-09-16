/**
 * App(ideas) Utility helper methods.
 *
 * Utility methods should be static
 */
/**
 *
 * This file is part of the APP(ideas) database abstraction project (AiDb).
 * Copyright 2013, APPideas
 *
 * AiDb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AiDb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AiDb (in the 'resources' directory). If not, see
 * <http://www.gnu.org/licenses/>.
 * http://appideas.com/abstract-your-database-introduction
 *
 */

#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>
#import <UIKit/UIKit.h>
#import <MobileCoreServices/MobileCoreServices.h>
#import "constants.h"

@class AppDelegate;
@class RaSubmissions;
@class AiDatabase;
@class RaConfig;

@interface AiUtil : NSObject <UIWebViewDelegate>
{
	NSInteger arbitraryNumber;
	UIWebView *webView;
}

@property (nonatomic,assign) NSInteger arbitraryNumber;
@property (nonatomic,retain) UIWebView *webView;

-(id)init;

/**
 * Strips characters from the input string
 *
 * @returns		A string with the given characters stripped
 * @since		20131221
 * @author		costmo
 * @param       charArray           An array of characters that are to be stripped
 * @param		inputString			The string from which characters are to be stripped
 */
+(NSString *)stripChars:(NSMutableArray *)charArray fromString:(NSString *)inputString;

/**
 * Using a Unix epoch date, returns a human-readable date. Use entries from constants.h for outputFormat specifiers
 *
 * @returns		A string representing the date or date/time in human-readable format
 * @since		20131221
 * @author		costmo
 * @param       epochDate           The epoch date to display
 * @param		outputFormat		The outout date format specifier. http://unicode.org/reports/tr35/tr35-6.html#Date_Format_Patterns
 */
+(NSString *)readableDateFromEpoch:(NSInteger)epochDate withFormat:(NSString *)outputFormat;

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
 */
+(void)debugString:(NSString *)message, ...;

/**
 * Whether the app is running on a tablet (YES) or phone (NO)
 *
 * @since		20140323
 * @author		costmo
 */
+(BOOL)isTablet;

/**
 * Strip whitespace from the beginning and end of a string
 *
 * @since		20140323
 * @author		costmo
 * @param		input	The input to trim
 */
+(NSString *)trimString:(NSString *)input;

/**
 * Gets the MD5 hash of a string
 *
 * @return		MD5 hash of a string
 * @since		20140323
 * @author		costmo
 * @param		input	The input hash
 */
+(NSString *)md5HashOfString:(NSString *)input;

/**
 * Gets the current Unix epoch timestamp
 *
 * @return		Number of seconds since 1970
 * @since		20140323
 * @author		costmo
 */
+(NSInteger)currentTimestamp;

/**
 * Return an elapsed time in string format from a number of seconds
 *
 * @return		A string
 * @since		20140323
 * @author		costmo
 */
+(NSString *)elapsedFromSeconds:(int)input;

/**
 * Make sure the needed view tag and storyboard ID arrays are available
 *
 * @returns		UIColor
 * @since		20141008
 * @author		costmo
 * @param		hexValue			The hex value to use for the color. e.g. ff000 for "red"
 */
+(UIColor *)UIColorFromRGB:(NSString *)hexValue;


/**
 * Replace newlines with paragraph breaks
 *
 * @returns		String
 * @since		20141008
 * @author		costmo
 * @param		inputString			The input to transform
 */
+(NSString *)nl2p:(NSString *)inputString;

/**
 * Replace newlines with html <br/>
 *
 * @returns		String
 * @since		20141008
 * @author		costmo
 * @param		inputString			The input to transform
 */
+(NSString *)nl2br:(NSString *)inputString;

/**
 * Create a human-readable string from a Unix epoch timestamp
 *
 * @returns		String
 * @since		20141008
 * @author		costmo
 * @param		epoch			The timestamp
 * @param		dateFormat		Format specifier
 */
+(NSString *)stringFromEpoch:(NSInteger)epoch withFormat:(NSString *)dateFormat;

/**
 * Strip HTML from input
 *
 * @returns		String
 * @since		20141008
 * @author		costmo
 * @param		str			The string to strip from
 */
+ (NSString *)stripTags:(NSString *)str;

/**
 * Verifies that an email address has the bare minum requirements to look valid
 *
 * @returns		String
 * @since		20141008
 * @author		costmo
 * @param		input			The address to check
 */
+(BOOL)emailIsValid:(NSString *)input;

/**
 * Returns the domain from an email address
 *
 * @returns		String
 * @since		20141008
 * @author		costmo
 * @param		input			The address split
 */
+(NSString *)splitDomainFromEmail:(NSString *)input;

/**
 * Programmatically set a constraint. Only deals with the simple, top, bottom, height and width
 *
 * @since		20141008
 * @author		costmo
 */
+(void)setConstraint:(NSString *)which forView:(UIView *)view toValue:(float)value;

/**
 * Display an error
 *
 * @since		20150802
 * @author		costmo
 * @param		message			The message to show
 * @param		title			The title to show
 * @param		caller			The caller of the error
 */
+(void)showError:(NSString *)message withTitle:(NSString *)title fromCaller:(id)caller;

/**
 * Get the md5 hash of a file
 *
 * @returns		NSString
 * @since		20140120
 * @author		costmo
 * @param		path		Full path to the file
 */
+(NSString *)md5HashOfPath:(NSString *)path;

/**
 * Get the sha1 hash of a file
 *
 * @returns		NSString
 * @since		20140120
 * @author		costmo
 * @param		path		Full path to the file
 */
+(NSString *)shaHashOfPath:(NSString *)path;

+(UIImage *)overlayImage:(UIImage *)image withColor:(UIColor *)color;

+(NSString *)stringFromLocalTimeEpoch:(NSInteger)epoch withFormat:(NSString *)dateFormat;

+(NSString *)mimeTypeForPath:(NSString *)path;
+(NSString *)generateBoundaryString;
+(NSData *)createPostBodyWithBoundary:(NSString *)boundary parameters:(NSDictionary *)parameters paths:(NSArray *)paths fieldName:(NSString *)fieldName;
+(void)setAspectRatio:(float)ratio forView:(UIView *)view;

+(NSMutableDictionary *)getHtmlEnvelope;
+(void)rotateToDeviceOrientation;
+(BOOL)isCameraAvailable;
+(BOOL)isPhotoLibraryAvailable;

+(NSString*) deviceName;

+(BOOL)isPhone;
+(NSString *)getIPAddress;

@end
