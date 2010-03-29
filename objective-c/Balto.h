/***
 * Copyright (c) 2009, 2010 Mark Sands. All rights reserved.
 * March, 27 2010
 *
 * bALt - Basic openAL Toolkit
 *
 * bALt - a lightweight OpenAL toolkit for Audio playback
 *	designed to encapsulate the low level audio handling
 *	and provide the user with a nice, easy to use
 *	high level application programming interface for
 *	sound rendering in their applications.
 *
 ***
 *
 * TODO:
 *
 ***/

#import <Foundation/Foundation.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>

/*
 * AduioPlayer
 * 	Main class for reading user audio files and producing the audio output
 * 	provides inherited member fuctions from MediaPlayer
 *
 */ 

@interface Balto : NSObject {
	
	ALCcontext *Context;
	ALCdevice *Device;
	
	enum {
		SOURCE_IN_USE,
		SOURCE_FREE
	} SOURCE_STATES;
	
	ALuint NUM_BUFFERS;
	
	ALuint Buffers[256];
	ALuint Sources[256];
	
	int altSourceData[256];
		
	int playCount;
	char* filename;
	
	NSMutableArray *audioFiles;
}

@property (nonatomic, retain) NSMutableArray* audioFiles;

- (id) initWithFiles:(NSMutableArray*)filenames andSize:(int)size;

- (void) Play:(int)index andLooping:(BOOL)looping;
- (void) Pause;
- (void) Stop;
- (void) SetVolume:(float)volume;	

- (void) Load;


- (BOOL) InitSources;
- (void) Delete;
- (void) CleanSources;
- (ALuint) GetFreeSource;

@end
