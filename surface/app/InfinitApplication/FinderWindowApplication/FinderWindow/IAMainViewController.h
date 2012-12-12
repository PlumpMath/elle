//
//  IAMainViewController.h
//  FinderWindowApplication
//
//  Created by infinit on 10/31/12.
//  Copyright (c) 2012 infinit. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "IASwaggersView.h"

@interface IAMainViewController : NSViewController

@property (retain) IBOutlet IASwaggersView* swaggers_view;

- (void)refresh;

@end
