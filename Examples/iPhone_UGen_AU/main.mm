#import <UIKit/UIKit.h>
#include "../../UGen/UGen.h"


// host class - must implement constructGraph
@interface UGenIPhoneDemoHost : UIKitAUIOHost
{ 
} 
@end

// view controller - the host class could go in here if you have only one view controller
@interface UGenIPhoneDemo : UIViewController
{	
	UIView *contentView;
	UILabel *labelView;
	NSTimer *timer;
}
-(void)updateCpuLabel: (NSTimer*)theTimer;
@end

// app delegate
@interface UGenAppDelegate : NSObject <UIApplicationDelegate> //UIKitAUIOHost
{ 
	UGenIPhoneDemoHost* host;
} 
@property (nonatomic, retain) UGenIPhoneDemoHost* host;
@end 



@implementation UGenIPhoneDemoHost

- (id)init
{
	if (self = [super init])
	{
		// other initialisation... none needed here
	}
	return self;
}

- (UGen)constructGraph:(UGen)input
{	
	return SinOsc::AR(1000, 0, 0.1);
}

@end



@implementation UGenIPhoneDemo

- (id)init
{
	if (self = [super init])
	{
		self.title = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];		
	}
	return self;
}

- (void)loadView
{		
	contentView = [[UIView alloc] initWithFrame: [[UIScreen mainScreen] applicationFrame]];
	
	//... and subviews e.g., sliders etc
	
	labelView = [[UILabel alloc] initWithFrame: CGRectMake(10, 10, 150, 20)];
	labelView.text = @"0.00%";
	[contentView addSubview: labelView];
	
		
	// Provide support for auto-rotation and resizing
	contentView.autoresizesSubviews = YES;
	contentView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);

	// Assign the view to the view controller
	self.view = contentView;
//    [contentView release];	
	
	timer = [NSTimer scheduledTimerWithTimeInterval:0.1 
											 target:self 
										   selector:@selector(updateCpuLabel:) 
										   userInfo:nil 
											repeats:YES];
	
}

-(void)updateCpuLabel: (NSTimer*)theTimer
{
	UGenAppDelegate* app = (UGenAppDelegate*)[[UIApplication sharedApplication] delegate];
	float cpu = [app.host getCpuUsage] * 100.f;
	labelView.text = [NSString stringWithFormat: @"%.2f%%", cpu]; 
}

// Allow the view to respond to iPhone Orientation changes
-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return YES;
}

-(void) dealloc
{	
	// add any further clean-up here
	[contentView release];
	[labelView release];
	[super dealloc];
}

@end




@implementation UGenAppDelegate 

@synthesize host;

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{ 
	UIWindow *window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]]; 
	UGenIPhoneDemo *root = [[UGenIPhoneDemo alloc] init];
	UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:root]; 
	[window addSubview:nav.view]; 
	[window makeKeyAndVisible]; 
		
	// initialise the audio host class and the audio (audio could be initialised later if required)
	host = [[UGenIPhoneDemoHost alloc] init];
//	[host setPreferredBufferSize: 2048];
//	[host setPreferredSampleRate: 44100];
	[host initAudio];
} 

- (void)applicationWillTerminate:(UIApplication *)application 
{ 
	// shutdown the audio host, this function will not return until a brief 
	// fade-out has been performed
	[host shutdown]; 
} 

- (void)dealloc 
{ 
	[super dealloc]; 
} 

@end 

int main(int argc, char *argv[]) 
{ 
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init]; 
	int retVal = UIApplicationMain(argc, argv, nil, @"UGenAppDelegate"); 
	[pool release]; 
	return retVal; 
};
