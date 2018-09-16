//
//  ApLoginViewController.m
//  AppideasLights
//
//  Created by Chris Ostmo on 9/14/18.
//  Copyright © 2018 Chris Ostmo. All rights reserved.
//

#import "ApLoginViewController.h"
#import "constants.h"
#import "AiUtil.h"

@interface ApLoginViewController ()

@end

@implementation ApLoginViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self requestConnectionPage];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)webView:(WKWebView *)webView didStartProvisionalNavigation:(WKNavigation *)navigation
{
    [AiUtil debugString:@"DSPN"];
}

-(void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation
{
     [AiUtil debugString:@"DFinishN"];
}

-(void)webView:(WKWebView *)webView didFailNavigation:(WKNavigation *)navigation withError:(NSError *)error
{
    [AiUtil debugString:@"DFailN"];
}

-(void)requestConnectionPage
{
    [AiUtil debugString:@"VDL 0"];
    
    self.webView.navigationDelegate = self;
    NSURL *nsurl=[NSURL URLWithString:_softApUrl];
    [AiUtil debugString:_softApUrl];
    NSURLRequest *nsrequest=[NSURLRequest requestWithURL:nsurl];
    [self.webView loadRequest:nsrequest];
    [self.view addSubview:self.webView];
    
    [AiUtil debugString:@"VDL 1"];
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
