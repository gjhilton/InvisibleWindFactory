#import "Cue.h"

static NSString * const IWFCueTitleCoderKey = @"title";

@implementation Cue

@synthesize title, command;

+(Cue *)cueWithTitle:(NSString *)title andCommand:(NSString *)command{
    Cue *result = [Cue new];
    result.command = command;
    result.title = title;
    
    return result;
}

-(BOOL)isLeaf{
    return YES;
}


#pragma mark NSCoding

- (NSArray *)mutableKeys
{
    return [NSArray arrayWithObjects:
			@"title",
			@"command",
            nil];
}

- (id)initWithCoder:(NSCoder *)coder
{
    self = [self init];
    if (self)
    {
        for (NSString *key in [self mutableKeys])
            [self setValue:[coder decodeObjectForKey:key] forKey:key];
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    for (NSString *key in [self mutableKeys])
    {
        [coder encodeObject:[self valueForKey:key] forKey:key];
    }
}

@end
