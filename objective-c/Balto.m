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

@implementation AudioPlayer

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
	
	- (id) init withFilename:(NSArray*)filename andSize:(int)size {

		for( int i = 0; i < size; i++ )
			audioFiles.push_back( filenames[i] );
			
		if ( !InitSources() )
			std::cerr << "Failed to initialize OpenAL!";
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

	dealloc?
	AudioPlayer::~AudioPlayer() {
		Delete();
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

	- (void) Load()
	{
		for ( int i = 0; i < (int)audioFiles.size(); i++ ) {
			Buffers[i] = loadWAVFromFile( audioFiles[ i ] );
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
	

	- (void) Play( int index, bool looping )
	{	
		CleanSources();
		
		int num = GetFreeSource();
		
		if ( num != -1 ) {
			altSourceData[num].INUSE = SOURCE_IN_USE;
			playCount++;

			// check if data hasn't been loaded into the Buffer
			ALint value; alGetBufferi( Buffers[ index ], AL_SIZE, &value );
			if ( value <= 0 )
				Buffers[index] = loadWAVFromFile( audioFiles[ index ] );

			// set the source to the associated buffer
			alSourceQueueBuffers( Sources[num], 1, &Buffers[ index ] );
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
	- (void) Pause( int index )
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
	- (void) Stop( int index )
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
	

	- (void) SetVolume( const int index,  const float volume)
	{
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
			alSourcei( Sources[i], AL_GAIN, volume > 0 ? 1.0 : volume );
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

	- (ALuint) GetFreeSource()
	{	
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			if ( altSourceData[i].INUSE == SOURCE_FREE )
				return i;
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

	- (void) CleanSources()
	{
		if ( playCount >= (int)NUM_BUFFERS/2 ) {
			ALenum state;

			for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
				if ( altSourceData[i].INUSE == SOURCE_IN_USE ) {
				alGetSourcei( Sources[i], AL_SOURCE_STATE, &state );
				if ( state != AL_PLAYING ) {
				  alSourceStop( Sources[i] );
				  alSourcei( Sources[i], AL_BUFFER, NULL );
				  altSourceData[i].INUSE = SOURCE_FREE;
				  playCount--;
			}}}
		}
	}
	

	/*
	 * InitSources()
	 * Last modified: 26Mar2010
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

	- (bool) InitSources()
	{
		playCount = 0;

		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			altSourceData[i].INUSE = SOURCE_FREE;
		}

		for ( int i = 0; i < (int)NUM_BUFFERS; i++) {
			for ( int j = 0; j < 3; j++ ) {
				altSource.position[i][j] = 0.0;
				altSource.velocity[i][j] = 0.0;
			}
		}

		for ( int i = 0; i < 3; i++ ) {
			altListener.position[i] = 0.0;
			altListener.velocity[i] = 0.0;
			altListener.orientation[i] = 0.0;			
		}
		altListener.orientation[2] = 1.0;
		altListener.orientation[4] = -1.0;

		Device = alcOpenDevice( (ALCchar*)"DirectSound3D" );
		if (Device == NULL)
			return false;

		Context = alcCreateContext(Device,NULL);
		alcMakeContextCurrent(Context);
		alGetError();

			// generate NUM_BUFFERS Buffers for use
		alGenBuffers(NUM_BUFFERS, Buffers);

			// set our empty buffers to NULL
		for ( int i = 0; i < (int)NUM_BUFFERS; i++) {
			Buffers[i] = NULL;
		}

			// generate NUM_BUFFERS Sources for use
		alGenSources(NUM_BUFFERS, Sources);

			// we don't queue our buffers here yet, only during the playback call
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			alSourcefv(Sources[i], AL_POSITION, altSource.position[0]);	
			alSourcefv(Sources[i], AL_VELOCITY, altSource.velocity[0]);
		}

		alListenerfv(AL_POSITION,       altListener.position);
		alListenerfv(AL_VELOCITY,       altListener.velocity);
		alListenerfv(AL_ORIENTATION, altListener.orientation);
	
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
 
	- (void) Delete() {

		alDeleteBuffers(NUM_BUFFERS, Buffers);
		alDeleteSources(NUM_BUFFERS, Sources);

		Context = alcGetCurrentContext();
		Device = alcGetContextsDevice(Context);
		
		alcMakeContextCurrent(NULL);
		alcDestroyContext(Context);
		alcCloseDevice(Device);	
	}
	

	/*  
	 * static
	 * loadWAVFromFile( filename )
	 * Last modified: 26Mar2010
	 * 
	 * A default function to load the wav
	 * via filename and return its buffer
	 * 
	 * Returns:     	ALuint buffer
	 * Parameters:
	 *    filename    in      the name of the wav file
	 */

	static ALuint
	loadWAVFromFile( char* filename ) {

		ALuint buffer = 0;

		WAVBuffer::SimpleWAVHeader header;
		WAVBuffer parser;

		char* data = parser.ReadWAV( filename, &header );
		buffer = parser.CreateBufferFromWav( data, header );

		return buffer;	
	}

@end