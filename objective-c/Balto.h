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
 *			create an MP3Buffer.h
 *			see: http://en.wikipedia.org/wiki/MP3#File_structure
 ***/

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
	
	// public: //
	
	ALCcontext *Context;
	ALCdevice *Device;
	
	enum {
		SOURCE_IN_USE,
		SOURCE_FREE
	} SOURCE_STATES;
	
	// private: //
	
	// 256
	ALuint NUM_BUFFERS;
	
	ALuint Buffers[256];
	ALuint Sources[256];
	
	NSMutableArray *SourceData; // INUSE
		
	int playCount;
	char* filename;
	
	NSMutableArray *audioFiles;
}

//@property (nonatomic) ALuint *Buffers;
//@property (nonatomic) ALuint *Sources;

// public:
- (id) init: withFilename:(NSArray*)filenames andSize:(int)size;

- (void) Play: withIndex:(int)index andLooping:(BOOL)looping;
- (void) Pause: withIndex:(int)index;
- (void) Stop: withIndex:(int)index;
- (void) SetVolume: withIndex:(int)index andVolume:(float)volume;	

- (void) Load;

// protected:

- (BOOL) InitSources;
- (void) Delete;
- (void) CleanSources;
- (ALuint) GetFreeSource;

@end
