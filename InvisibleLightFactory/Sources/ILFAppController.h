@import Foundation;
#import "ORSSerialPort.h"

@class ORSSerialPortManager;

#if (MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_7)
@protocol NSUserNotificationCenterDelegate <NSObject>
@end
#endif

@interface ILFAppController : NSObject <ORSSerialPortDelegate, NSUserNotificationCenterDelegate>

- (IBAction)cue:(id)sender;
- (IBAction)send:(id)sender;
- (IBAction)openOrClosePort:(id)sender;
- (IBAction)soft:(id)sender;

@property (unsafe_unretained) IBOutlet NSTextField *delayTF;
@property (unsafe_unretained) IBOutlet NSTextField *repeatsTF;


@property (unsafe_unretained) IBOutlet NSTextView *commandsTextView;
@property (unsafe_unretained) IBOutlet NSTextView *receivedDataTextView;
@property (unsafe_unretained) IBOutlet NSButton *openCloseButton;

@property (nonatomic, strong) ORSSerialPortManager *serialPortManager;
@property (nonatomic, strong) ORSSerialPort *serialPort;
@property (nonatomic, strong) NSArray *availableBaudRates;

@property BOOL flipFlop;

@end
