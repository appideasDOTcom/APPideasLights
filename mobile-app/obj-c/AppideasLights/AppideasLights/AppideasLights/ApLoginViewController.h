//
//  ApLoginViewController.h
//  AppideasLights
//
//  Created by Chris Ostmo on 9/14/18.
//  Copyright © 2018 Chris Ostmo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>

@interface ApLoginViewController : UIViewController <WKNavigationDelegate>
{
    
}

@property (weak, nonatomic) IBOutlet WKWebView *webView;


-(void)requestConnectionPage;

@end
