#import "ILFAppDelegate.h"
#import "ORSSerialPortManager.h"
#import "ORSSerialPort.h"
#import "Cue.h"

@interface ILFAppDelegate()

@property (weak) IBOutlet NSOutlineView *outlineView;
@property (weak) IBOutlet NSTreeController *cuesController;

- (IBAction)saveNewFile:(id)sender;


@end

@implementation ILFAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    self.outlineView.delegate = self;
    self.outlineView.dataSource = self;
    self.outlineView.floatsGroupRows = NO; // Prevent a sticky header
										   //[self addData];
	[self openExistingDocument];

}

- (void)applicationWillTerminate:(NSNotification *)notification
{
	NSArray *ports = [[ORSSerialPortManager sharedSerialPortManager] availablePorts];
	for (ORSSerialPort *port in ports) { [port close]; } 
}

#pragma mark - Add data

/*
- (void) addData{

    // `children` and `isLeaf` have to be configured for the Tree Controller in IB
    NSMutableDictionary *root = @{@"title": @"CUES",
                                  @"isLeaf": @(NO),
                                  @"children":@[
                                          [Cue cueWithTitle:@"one" andCommand:@"cFF0000"],
                                          [Cue cueWithTitle:@"two" andCommand:@"c00FF00"],
                                          [Cue cueWithTitle:@"three" andCommand:@"s10"]
                                          ].mutableCopy
                                  }.mutableCopy;

    [self.cuesController addObject:root];
}
 */


- (IBAction)addClicked:(id)sender {
    NSUInteger indexArr[] = {0,0};

    [self.cuesController insertObject:[Cue new] atArrangedObjectIndexPath:[NSIndexPath indexPathWithIndexes:indexArr length:2]];
}



#pragma mark - Helpers

- (BOOL) isHeader:(id)item{

    if([item isKindOfClass:[NSTreeNode class]]){
        return ![((NSTreeNode *)item).representedObject isKindOfClass:[Cue class]];
    } else {
        return ![item isKindOfClass:[Cue class]];
    }
}


#pragma mark - NSOutlineViewDelegate

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectItem:(id)item{
    return ![self isHeader:item];
}

- (NSView *)outlineView:(NSOutlineView *)outlineView viewForTableColumn:(NSTableColumn *)tableColumn item:(id)item {

    if ([self isHeader:item]) {
        return [outlineView makeViewWithIdentifier:@"HeaderCell" owner:self];
    } else {
        return [outlineView makeViewWithIdentifier:@"DataCell" owner:self];
    }
}

- (BOOL)outlineView:(NSOutlineView *)outlineView isGroupItem:(id)item{
    // This converts a group to a header which influences its style
    return [self isHeader:item];
}


#pragma mark - Drag & Drop

- (id <NSPasteboardWriting>)outlineView:(NSOutlineView *)outlineView pasteboardWriterForItem:(id)item{
    // No dragging if <some condition isn't met>
    BOOL dragAllowed = YES;
    if (!dragAllowed)  {
        return nil;
    }

    Cue *book = (Cue *)(((NSTreeNode *)item).representedObject);
    NSString *identifier = book.title;

    NSPasteboardItem *pboardItem = [[NSPasteboardItem alloc] init];
    [pboardItem setString:identifier forType: @"public.text"];

    return pboardItem;
}


- (NSDragOperation)outlineView:(NSOutlineView *)outlineView validateDrop:(id < NSDraggingInfo >)info proposedItem:(id)targetItem proposedChildIndex:(NSInteger)index{

    BOOL canDrag = index >= 0 && targetItem;

    if (canDrag) {
        return NSDragOperationMove;
    }else {
        return NSDragOperationNone;
    }
}


- (BOOL)outlineView:(NSOutlineView *)outlineView acceptDrop:(id < NSDraggingInfo >)info item:(id)targetItem childIndex:(NSInteger)index{

    NSPasteboard *p = [info draggingPasteboard];
    NSString *title = [p stringForType:@"public.text"];
    NSTreeNode *sourceNode;

    for(NSTreeNode *b in [targetItem childNodes]){
        if ([[[b representedObject] title] isEqualToString:title]){
            sourceNode = b;
        }
    }

    if(!sourceNode){
        // Not found
        return NO;
    }

    NSUInteger indexArr[] = {0,index};
    NSIndexPath *toIndexPATH =[NSIndexPath indexPathWithIndexes:indexArr length:2];

    [self.cuesController moveNode:sourceNode toIndexPath:toIndexPATH];

    return YES;
}


#pragma mark load and save

- (void)openExistingDocument{
	NSOpenPanel* panel = [NSOpenPanel openPanel];

	[panel beginWithCompletionHandler:^(NSInteger result){
		if (result == NSFileHandlingPanelOKButton) {
			NSURL *docURL = [[panel URLs] objectAtIndex:0];
			NSMutableDictionary *root = [[NSKeyedUnarchiver unarchiveObjectWithFile:[docURL path]] mutableCopy];

			[self.cuesController addObject:root];
			[self.outlineView expandItem:[self.outlineView itemAtRow:0]];
			[self.outlineView selectRowIndexes:[NSIndexSet indexSetWithIndex:1] byExtendingSelection:NO];
			[self.outlineView registerForDraggedTypes: [NSArray arrayWithObject: @"public.text"]];
		}
	}];
}


- (IBAction)saveNewFile:(id)sender {
	NSSavePanel*    panel = [NSSavePanel savePanel];
	[panel setNameFieldStringValue:@"new.cues"];
	[panel beginWithCompletionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton)
        {
            NSURL*  theFile = [panel URL];

			// ugh. who knew this would be so hard?
			NSTreeNode *root = [[self.cuesController arrangedObjects] childNodes][0];
			NSArray *cueNodes = root.childNodes;
			NSMutableArray *cues = [[NSMutableArray alloc]init];
			for (NSTreeNode *eachObject in cueNodes) {
				NSLog(@"Object: %@", [eachObject representedObject]);
				[cues addObject:[eachObject representedObject]];
			}
			NSMutableDictionary *saveData = @{@"title": @"CUES",
												@"isLeaf": @(NO),
												@"children":cues
												}.mutableCopy;

			[NSKeyedArchiver archiveRootObject:saveData toFile:[theFile path]];
			/*
			NSLog(@"attempting to save cues");
			if ([NSKeyedArchiver archiveRootObject:saveData toFile:[theFile path]]) {
				NSLog(@"wrote successfully");
			} else {
				NSLog(@"error writing file");
			}
			 */
        }
    }];
}


@end
