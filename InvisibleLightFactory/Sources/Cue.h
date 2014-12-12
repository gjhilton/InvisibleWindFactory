#import <Foundation/Foundation.h>

@interface Cue : NSObject <NSCoding>

@property (copy) NSString *title;
@property (copy) NSString *command;

+ (Cue*) cueWithTitle:(NSString *)title andCommand:(NSString *) command;

@property (readonly) BOOL isLeaf;

@end
