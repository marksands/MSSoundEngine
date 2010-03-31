/***
 * Copyright (c) 2009, 2010 Mark Sands. All rights reserved.
 *
 * This sample file demonstrates very basic usage of Balto.
 * If you're using this to develop for the iPhone, here's
 * an example you could use to load a file into an array,
 * and pass its contents into Balto in order to play sound.
 *
 ***/

#import <UIKit/UIKit.h>
#import "Balto.h"

int main(int argc, char *argv[]) {
    
	NSString *filePath = [[NSBundle mainBundle] pathForResource:@"sample" ofType:@"wav"];  
		
	NSMutableArray *filenames = [[NSMutableArray alloc] initWithCapacity:1];
	[filenames addObject:filePath];
			
	Balto *b = [[Balto alloc] initWithFiles:filenames];
	[b Play:0 andLooping:NO];
	
	// ... let the song play out
	sleep(30);
	
	[b Stop];
	
	[filenames release];
	[b release];
		
	return 0;
}