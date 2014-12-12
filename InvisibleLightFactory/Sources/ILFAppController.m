#import "ILFAppController.h"
#import "ORSSerialPortManager.h"

@implementation ILFAppController

- (instancetype)init
{
    self = [super init];
    if (self)
	{
        self.serialPortManager = [ORSSerialPortManager sharedSerialPortManager];
		self.availableBaudRates = @[@300, @1200, @2400, @4800, @9600, @14400, @19200, @28800, @38400, @57600, @115200, @230400];
		self.flipFlop = false;
		
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		[nc addObserver:self selector:@selector(serialPortsWereConnected:) name:ORSSerialPortsWereConnectedNotification object:nil];
		[nc addObserver:self selector:@selector(serialPortsWereDisconnected:) name:ORSSerialPortsWereDisconnectedNotification object:nil];

#if (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_7)
		[[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
#endif
    }
    return self;
}

- (void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - Actions

- (IBAction)soft:(id)sender
{
	NSString *bang = @"#";
	if (self.flipFlop == false){
		bang = [bang stringByAppendingString:@"8"];
	} else{
		bang = [bang stringByAppendingString:@"9"];
	}
	self.flipFlop = !self.flipFlop;

	[self serialPrint:bang];
}

- (IBAction)cue:(id)sender
{
	NSString *bang = @"!";
	if (self.flipFlop == false){
		bang = [bang stringByAppendingString:@"1"];
	} else{
		bang = [bang stringByAppendingString:@"0"];
	}
	self.flipFlop = !self.flipFlop;

	[self serialPrint:bang];
}

- (IBAction)send:(id)sender
{
	[self serialPrint:self.commandsTextView.string];
}

- (void)serialPrint:(NSString *)messages
{
	NSInteger nSends = self.repeatsTF.integerValue;
	NSInteger delayMillis = self.delayTF.integerValue;
	NSArray *commands = [messages componentsSeparatedByString:@"\n"];
	[commands enumerateObjectsUsingBlock:^ (id obj, NSUInteger idx, BOOL *stop) {
		NSString *sendStr = [obj stringByAppendingString:@"\n"];
		NSData *dataToSend = [sendStr dataUsingEncoding:NSUTF8StringEncoding];
		for (int i=0; i <nSends; i++){
		[self.serialPort sendData:dataToSend];
			// NSLog(@"send: %@ (%i)", sendStr, i+1);
		if (delayMillis > 0) {
			//NSLog(@"%f", delayMillis / 1000.f);
			NSTimeInterval t = delayMillis / 1000.f;
			[NSThread sleepForTimeInterval:t];
		}
		}
	}];
}

- (IBAction)openOrClosePort:(id)sender
{
	self.serialPort.isOpen ? [self.serialPort close] : [self.serialPort open];
}


#pragma mark - ORSSerialPortDelegate Methods

- (void)serialPortWasOpened:(ORSSerialPort *)serialPort
{
	self.openCloseButton.title = @"Close";
}

- (void)serialPortWasClosed:(ORSSerialPort *)serialPort
{
	self.openCloseButton.title = @"Open";
}

- (void)serialPort:(ORSSerialPort *)serialPort didReceiveData:(NSData *)data
{
	NSString *string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
	if ([string length] == 0) return;
	[self.receivedDataTextView.textStorage.mutableString appendString:string];
	[self.receivedDataTextView setNeedsDisplay:YES];
}

- (void)serialPortWasRemovedFromSystem:(ORSSerialPort *)serialPort;
{
	// After a serial port is removed from the system, it is invalid and we must discard any references to it
	self.serialPort = nil;
	self.openCloseButton.title = @"Open";
}

- (void)serialPort:(ORSSerialPort *)serialPort didEncounterError:(NSError *)error
{
	NSLog(@"Serial port %@ encountered an error: %@", serialPort, error);
}

#pragma mark - NSUserNotificationCenterDelegate

#if (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_7)

- (void)userNotificationCenter:(NSUserNotificationCenter *)center didDeliverNotification:(NSUserNotification *)notification
{
	dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, 3.0 * NSEC_PER_SEC);
	dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
		[center removeDeliveredNotification:notification];
	});
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
	return YES;
}

#endif

#pragma mark - Notifications

- (void)serialPortsWereConnected:(NSNotification *)notification
{
	NSArray *connectedPorts = [notification userInfo][ORSConnectedSerialPortsKey];
	NSLog(@"Ports were connected: %@", connectedPorts);
	[self postUserNotificationForConnectedPorts:connectedPorts];
}

- (void)serialPortsWereDisconnected:(NSNotification *)notification
{
	NSArray *disconnectedPorts = [notification userInfo][ORSDisconnectedSerialPortsKey];
	NSLog(@"Ports were disconnected: %@", disconnectedPorts);
	[self postUserNotificationForDisconnectedPorts:disconnectedPorts];
	
}

- (void)postUserNotificationForConnectedPorts:(NSArray *)connectedPorts
{
#if (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_7)
	if (!NSClassFromString(@"NSUserNotificationCenter")) return;
	
	NSUserNotificationCenter *unc = [NSUserNotificationCenter defaultUserNotificationCenter];
	for (ORSSerialPort *port in connectedPorts)
	{
		NSUserNotification *userNote = [[NSUserNotification alloc] init];
		userNote.title = NSLocalizedString(@"Serial Port Connected", @"Serial Port Connected");
		NSString *informativeTextFormat = NSLocalizedString(@"Serial Port %@ was connected to your Mac.", @"Serial port connected user notification informative text");
		userNote.informativeText = [NSString stringWithFormat:informativeTextFormat, port.name];
		userNote.soundName = nil;
		[unc deliverNotification:userNote];
	}
#endif
}

- (void)postUserNotificationForDisconnectedPorts:(NSArray *)disconnectedPorts
{
#if (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_7)
	if (!NSClassFromString(@"NSUserNotificationCenter")) return;
	
	NSUserNotificationCenter *unc = [NSUserNotificationCenter defaultUserNotificationCenter];
	for (ORSSerialPort *port in disconnectedPorts)
	{
		NSUserNotification *userNote = [[NSUserNotification alloc] init];
		userNote.title = NSLocalizedString(@"Serial Port Disconnected", @"Serial Port Disconnected");
		NSString *informativeTextFormat = NSLocalizedString(@"Serial Port %@ was disconnected from your Mac.", @"Serial port disconnected user notification informative text");
		userNote.informativeText = [NSString stringWithFormat:informativeTextFormat, port.name];
		userNote.soundName = nil;
		[unc deliverNotification:userNote];
	}
#endif
}


#pragma mark - Properties

- (void)setSerialPort:(ORSSerialPort *)port
{
	if (port != _serialPort)
	{
		[_serialPort close];
		_serialPort.delegate = nil;
		
		_serialPort = port;
		
		_serialPort.delegate = self;
	}
}

@end
