/***
 * Copyright (c) 2009 Mark Sands. All rights reserved.
 * September, 4 2009
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

@interface AudioPlayer : NSObject {

	// public: //
	
		ALCcontext *Context;
		ALCdevice *Device;

		enum SOURCE_STATES {
		  SOURCE_IN_USE,
			SOURCE_FREE
		};

	// private: //
	
	 		// 256
		ALuint NUM_BUFFERS;

 		NSMutableArray *Buffers;
 		NSMutableArray *Sources;

		typedef struct {
		  ALuint INUSE;				
		} altSourceData_;
		altSourceData_ altSourceData[256];

		typedef struct {
		  ALfloat position[256][3];
		  ALfloat velocity[256][3];
		} altSource_;
		altSource_ altSource;

		typedef struct {
		  ALfloat position[3];
		  ALfloat velocity[3];
		  ALfloat orientation[6];
		} altListener_;
		altListener_ altListener;

		int playCount;
		char* filename;
		
		NSMutableDictionary *audioFiles;
}

@property (nonatomic, retain) NSMutableArray *Buffers;
@property (nonatomic, retain) NSMutableArray *Sources;
@property (nonatomic, retain) NSMutableDictionary *audioFiles;

	// public:
	
- (id) init withFilename:(NSArray*)filenames, andSize:(int)size;

- (void) Play( const int index = 1, bool looping = false);
- (void) Pause( const int index = 1);
- (void) Stop( const int index = 1);
- (void) SetVolume( const int index = 1,  const float volume = 1.0f);	

- (void) Load();

	// protected:

- (BOOL) InitSources();
- (void) Delete();
- (void) CleanSources();
- (ALuint) GetFreeSource();

@end

#endif /* end of include guard: BALT_H_KBA7BAQ2 */