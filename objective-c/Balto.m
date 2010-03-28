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

#import "Balto.h"

// WAVheader
typedef struct SimpleWAVHeader {
	char  riff[4];
	unsigned int riffSize;
	char  wave[4];
	char  fmt[4];
	unsigned int fmtSize;
	unsigned short format;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char  data[4];
	unsigned int dataSize;
} SimpleWAVHeader;		


//
// ReadWAV( filename, header )
// Last modified: 267Mar2010
//
// the WAVBuffer method that reads the wav into the header
// and returns the buffer data from the wav file
//
// Returns:     char* buffer (data)
// Parameters:
//      filename    in      the name of the wav file
//      header      in      the wav header struct
//

char* ReadWAV( char *filename, SimpleWAVHeader *header ) {
	
	char* buffer = 0;
	FILE* file;
	
	if( file = fopen(filename, "rb") ) {
		
		fread( header, sizeof(SimpleWAVHeader), 1, file );
		
		if (!( 
			  memcmp("RIFF",header->riff,4) || 
			  memcmp("WAVE",header->wave,4) || 
			  memcmp("fmt ",header->fmt,4)  || 
			  memcmp("data",header->data,4)
			  ))
		{	
			buffer = (char*)malloc(header->dataSize);
			
			fread( buffer, header->dataSize, 1, file );
			fclose(file);
			
			return buffer;
		}
		
		free(buffer);
		fclose(file);
	}
	else {
		printf("Audio rendering aborted! Failed to read %s\n", filename);
	}
	
	return 0;
}


//
// CreateBufferFromWav( data, header )
// Last modified: 27Mar2010
//
// the WAVBuffer method that generates and creates
// an audio buffer from the wav data
//
// Returns:     ALuint buffer
// Parameters:
//      data    in   the data buffer from the wav file
//      header  in   the wav header struct
//

ALuint CreateBufferFromWav( char* data, SimpleWAVHeader header ) {
	
	ALuint buffer = 0;
	ALuint format = 0;
	
	switch ( header.bitsPerSample ) {
		case 8:
			format = (header.channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
			break;
		case 16:
			format = (header.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
			break;
		default:
			return 0;
	}
	
	alGenBuffers( 1, &buffer );
	alBufferData( buffer, format, data, header.dataSize, header.samplesPerSec );
	
		// memory cleanup of pre-allocated data
	free(data);
	
	return buffer;
}


/*  
 * static
 * loadWAVFromFile( filename )
 * Last modified: 27Mar2010
 * 
 * A default function to load the wav
 * via filename and return its buffer
 * 
 * Returns:     	ALuint buffer
 * Parameters:
 *    filename    in      the name of the wav file
 */

ALuint loadWAVFromFile( char* filename ) {
	
	ALuint buffer = 0;
	
	SimpleWAVHeader header;
	
	char* data = ReadWAV( filename, &header );
	buffer = CreateBufferFromWav( data, header );
	
	return buffer;	
}


/*
 *	Impelmentation
 *	of the Balto Class
 8
 */

@implementation Balto


/*
 * AudioPlayer( filenames, size )
 * Last modified: 26Mar2010
 *
 * Default constructor lodas the filename array of songs.
 * Sets the NUM_BUFFERS to 256 maximum allowed buffers
 * which I believe is the absolute max OpenAL allows.
 *
 * Returns:              <none>
 * Parameters:
 *      filename  				   in   array of all the wav filenames
 *      size      (optional) in   the size of the array, default to 1
 *
 */

- (id) initWithFilename:(NSMutableArray*)filenames andSize:(int)size {
	
	for(NSString *fname in filenames)
		[audioFiles addObject:fname];
	
	if ( ![self InitSources] )
		NSLog(@"Failed to initialize OpenAL");
	
	// Load files into buffers
	[self Load];
	
	return self;
}


/*
 * ~AudioPlayer()
 * Last modified: 26Mar2010
 *
 * Default destructor calls the Delete() method
 *
 * Returns:     <none>
 * Parameters: 	<none>
 *
 */

- (void) dealloc {
	[super dealloc];
	[self Delete];
}


/*
 * Load()
 * Last modified: 26Mar2010
 *
 * Method to load all the audio sources into the Buffers
 *
 * Returns:     <none>
 * Parameters: 	
 *							<none>
 */

- (void) Load
{
	int i = 0;
	for(NSString *fname in audioFiles) {
		Buffers[i] = [loadWAVFromFile fname];
		i++;
	}
}


/*
 * Play( index, looping )
 * Last modified: 26Mar2010
 *
 * Method to play the specified file index
 * This is where we want to queue our buffers
 * 
 * Returns:            <none>
 * Parameters: 	
 * 		index								 	 in		specified source from filenames array
 * 		optional	(optional) 	 in		specifies wether or not to loop the source
 *
 */


- (void) PlayWithIndex:(int)index andLooping:(BOOL)looping
{	
	[self CleanSources];
	
	int num = [self GetFreeSource];
	
	if ( num != -1 ) {
		// altSourceData[num].INUSE = SOURCE_IN_USE;
		playCount++;
		
		// check if data hasn't been loaded into the Buffer
		ALint value;
		alGetBufferi( Buffers[index], AL_SIZE, &value );
		if ( value <= 0 )
			Buffers[num] = loadWAVFromFile([audioFiles objectAtIndex:index]);
		
		// set the source to the associated buffer
		alSourceQueueBuffers( Sources[num], 1, &Buffers[index] );
		alSourcei(Sources[num], AL_LOOPING, (looping ? AL_TRUE : AL_FALSE) );
		alSourcePlay(Sources[num]);
	}
}



/*
 * Pause( index )
 * Last modified: 26Mar2010
 *
 * Pauses all sounds in the buffer
 *
 * Returns:      <none>
 * 		Parameters:
 * 		index			  	 in		specified source from filenames array
 *
 */


// OpenAL Pause Sound, pauses all sounds in the buffer
- (void) PauseWithIndex:(int)index
{
	for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
		alSourcePause( Sources[i] );
}


/*
 * Stop( index )
 * Last modified: 26Mar2010
 *
 * stops all sounds in the buffer
 *
 * Returns:     <none>
 * Parameters:
 * 		index			    in		specified source from filenames array
 *
 */

// OpenAL Stop sound, stops all sounds in the buffer
- (void) StopWithindex:(int)index
{
	for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
		alSourceStop( Sources[i] );
}


/*
 * SetVolume( index, volume )
 * Last modified: 26Mar2010
 *
 * sets the volume from 0.0 to 1.0 (max)
 *
 * Returns:     <none>
 * Parameters:
 * 		index		    in		specified source from filenames array
 * 		volume	    in	  the gain of the volume, 0.0 to 1.0
 *
 */

- (void) SetVolumeWithIndex:(int)index andVolume:(float)volume
{
	for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
		alSourcei( Sources[i], AL_GAIN, volume );
}


/*
 * GetFreeSource()
 * Last modified: 26Mar2010
 *
 * Returns a free source from the sources buffer
 *
 * Returns:     <none>
 * Parameters:
 * 							<none>
 */

- (ALuint) GetFreeSource
{	
	for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
		//if ( altSourceData[i].INUSE == SOURCE_FREE )
		//	return i;
	}
	return -1;
}


/*
 * CleanSources()
 * Last modified: 26Mar2010
 *
 * Resets finished sources to a free and available state.
 * Be sure to unqueue the unplayed buffers here.
 * Here I unqueue the beuffer by specifying the AL_BUFFER
 * state to NULL.
 *
 * Returns:     <none>
 * Parameters:
 *							<none>
 */

- (void) CleanSources
{
	if ( playCount >= (int)NUM_BUFFERS/2 ) {
		ALenum state;
		
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			//if ( altSourceData[i].INUSE == SOURCE_IN_USE ) {
				alGetSourcei( Sources[i], AL_SOURCE_STATE, &state);
				if ( state != AL_PLAYING ) {
					alSourceStop( Sources[i] );
					alSourcei( Sources[i], AL_BUFFER, NULL );
					//altSourceData[i].INUSE = SOURCE_FREE;
					playCount--;
				}}//}
	}
}


/*
 * InitSources()
 * Last modified: 27Mar2010
 *
 * Right now this is a nasty method
 * to initialize the sources from the
 * filenames array.
 * A lot of OpenAL initialize is done
 * here, which also sets up each 
 * listener and orientation position.
 * Also, be sure not to queue the buffers
 * here. That is only done during playback.
 *
 * Returns:   	bool		true or false if an error occurs
 * Parameters:
 *							<none>
 */

- (BOOL) InitSources
{
	playCount = 0;
	
	for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
		//altSourceData[i].INUSE = SOURCE_FREE;
	}
	
	
	Device = alcOpenDevice( (ALCchar*)"DirectSound3D" );
	if (Device == NULL)
		return false;
	
	Context = alcCreateContext(Device,NULL);
	alcMakeContextCurrent(Context);
	alGetError();
	
	// generate NUM_BUFFERS Buffers for use
	alGenBuffers(NUM_BUFFERS, Buffers);
	
	
	const ALfloat position[3] = { 0.0f, 0.0f, 0.0f };
	const ALfloat velocity[3] = { 0.0f, 0.0f, 0.0f };	
	const ALfloat orientation[5] = { 0.0f, 0.0f, 1.0f, 0.0f, -1.0f };

	
	// set our empty buffers to NULL
	for ( int i = 0; i < (int)NUM_BUFFERS; i++) {
		Buffers[i] = NULL;
	}
	
	// generate NUM_BUFFERS Sources for use
	alGenSources(NUM_BUFFERS, Sources);
	
	// we don't queue our buffers here yet, only during the playback call
	for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
		alSourcefv(Sources[i], AL_POSITION, position);	
		alSourcefv(Sources[i], AL_VELOCITY, velocity);
	}
	
	alListenerfv(AL_POSITION, position);
	alListenerfv(AL_VELOCITY, velocity);
	alListenerfv(AL_ORIENTATION, orientation);
	
	if( alGetError() != AL_NO_ERROR)
		return AL_FALSE;
	
	return true;
}

/*
 * Delete()
 * Last modified: 26Mar2010
 *
 * Deletes the OpenAL buffers and sources by
 * calling the OpenAL methods, also destorys
 * the playback device
 * 
 * Returns:     <none>
 * Parameters:
 *    		    <none>
 */

- (void) Delete
{
	
	alDeleteBuffers(NUM_BUFFERS, Buffers);
	alDeleteSources(NUM_BUFFERS, Sources);
	
	Context = alcGetCurrentContext();
	Device = alcGetContextsDevice(Context);
	
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);	
}

@end