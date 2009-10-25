//***************************************************************************//
// Copyright (c) 2009 Mark Sands. All rights reserved.
// September, 4 2009
//
// Audio Player - a lightweight OpenAL toolkit for Audio playback
//
//***************************************************************************//

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#if defined( __WIN32__ ) || defined( _WIN32 )
  #include "al.h"
  #include "alc.h"	
#elif defined( __APPLE_CC__)
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#else
  #include <AL/al.h>
  #include <AL/alc.h>
#endif

#include <iostream>
#include <vector>
#include "MediaPlayer.h"


//***************************************************************************//
// AduioPlayer
//	Main class for reading user audio files and producing the audio output
//  provides inherited member fuctions from MediaPlayer
// 
// TO-DO:
//  

	class AudioPlayer: public MediaPlayer {

		public:
			AudioPlayer( char* filenames[],  const int size = 1 );
			virtual ~AudioPlayer();

			void Play( const int index = 1, bool looping = false);
			void Pause( const int index = 1);
			void Stop( const int index = 1);
			void SetVolume( const int index = 1,  const float volume = 1.0f);	

		protected:
			bool InitSources();
			void Delete();
	
			void CleanSources();
			ALuint GetFreeSource();
		
			ALCcontext *Context;
			ALCdevice *Device;

			enum BUFFER_STATES {
				BUFFER_IN_USE = 0x000001,
				BUFFER_FREE   = 0x000002,
			};
		
		private:
			ALuint NUM_BUFFERS;
			ALuint *Buffers, *Sources;
			int playCount;

			// The following to replace the following
			struct Source {
			  ALfloat (*SourcesPos)[3];
			  ALfloat (*SourcesVel)[3];
			};
			struct Listener {
			  ALfloat position[3];
			  ALfloat velocity[3];
			  ALfloat orientation[6];
			};

			ALfloat (*SourcesPos)[3];
			ALfloat (*SourcesVel)[3];
			
			ALfloat ListenerPos[3];
			ALfloat ListenerVel[3];
			ALfloat ListenerOri[6];
			
			char* filename;
			std::vector<char*> audioFiles;
			std::vector<int> inuse;
	};

	//
	// AudioPlayer( filenames, size )
	// Last modified: 16Sep2009
	//
	// Default constructor lodas the filename array of songs
	//
	// Returns:              <none>
	// Parameters:
	//      filename  				   in   array of all the wav filenames
	//      size      (optional) in   the size of the array, default to 1
	//
	
	AudioPlayer::AudioPlayer(char *filenames[], const int size) {

		for( int i = 0; i < size; i++ )
			audioFiles.push_back( filenames[i] );
			
		if ( !InitSources() )
			std::cerr << "Failed to initialize OpenAL!";
	}



	//
	// ~AudioPlayer()
	// Last modified: 16Sep2009
	//
	// Default destructor calls the Delete() method
	//
	// Returns:     <none>
	// Parameters: 	<none>
	//

	AudioPlayer::~AudioPlayer() {
		Delete();
	}

	//
	// Play( index, looping )
	// Last modified: 25Oct2009
	//
	// Method to play the specified file index
	// This is where we want to queue our buffers
	//
	// Returns:     <none>
	// Parameters: 	
	//		index								 in		specified source from filenames array
	//		optional	(optional) in		specifies wether or not to loop the source
	//

	void AudioPlayer::Play( int index, bool looping ) {
		
		CleanSources();
		
		int num = GetFreeSource();
		
		if ( num != -1 ) {
			inuse[num] = BUFFER_IN_USE;
			playCount++;

			ALuint buffer = loadWAVFromFile( audioFiles[ fileinex ] );

			alSourceQueueBuffers( Sources[num], 1, &buffer );
			alSourcei(Sources[num], AL_LOOPING, (looping ? AL_TRUE : AL_FALSE) );
			alSourcePlay(Sources[num]);
		}
	}
	
	

	//
	// Pause( index )
	// Last modified: 17Oct2009
	//
	// Pauses all sounds in the buffer
	//
	// Returns:      <none>
	// Parameters:
	//		index			  	 in		specified source from filenames array
	//


	// OpenAL Pause Sound, pauses all sounds in the buffer
	void AudioPlayer::Pause( int index ) {
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
			alSourcePause( Sources[i] );
	}


	//
	// Stop( index )
	// Last modified: 17Oct2009
	//
	// stops all sounds in the buffer
	//
	// Returns:     <none>
	// Parameters:
	//		index			    in		specified source from filenames array
	//

	// OpenAL Stop sound, stops all sounds in the buffer
	void AudioPlayer::Stop( int index ) {
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
			alSourceStop( Sources[i] );
	}
	

	//
	// SetVolume( index, volume )
	// Last modified: 17Oct2009
	//
	// sets the volume from 0.0 to 1.0 (max)
	//
	// Returns:     <none>
	// Parameters:
	//		index			    in		specified source from filenames array
	//	 volume			    in	  the gain of the volume, 0.0 to 1.0
	//

	void AudioPlayer::SetVolume( const int index,  const float volume) {
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
			alSourcei( Sources[i], AL_GAIN, volume > 0 ? 1.0 : volume );
	}


	//
	// GetFreeSource()
	// Last modified: 17Oct2009
	//
	// Returns a free source from the sources buffer
	//
	// Returns:     <none>
	// Parameters:
	//							<none>
	//

	ALuint AudioPlayer::GetFreeSource() {
		
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			if ( inuse[i] == BUFFER_FREE )
				return i;
		}
		
		return -1;
	}
	

	//
	// CleanSources()
	// Last modified: 17Oct2009
	//
	// Resets finished sources to a free and available state
	// Be sure to unqueue the unplayed buffers here.
	// Here I unqueue the beuffer by specifying the AL_BUFFER
	// state to NULL.
	//
	// Returns:     <none>
	// Parameters:
	//							<none>
	//

	// need to unqueue unplayed buffers here
	void AudioPlayer::CleanSources()
	{
		if ( playCount >= (int)NUM_BUFFERS/2 ) {
			ALenum state;

			for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
				if ( inuse[i] == BUFFER_IN_USE ) {
				alGetSourcei( Sources[i], AL_SOURCE_STATE, &state );
				if ( state != AL_PLAYING ) {
				  alSourceStop( Sources[i] );
				  alSourcei( Sources[i], AL_BUFFER, NULL );
				  inuse[i] = BUFFER_FREE;
				  playCount--;
			}}}
		}
	}
	

	//
	// InitSources()
	// Last modified: 17Oct2009
	//
	//   Right now this is a nasty method
	//   to initialize the sources from the
	//   filenames array.
	//   A lot of OpenAL initialize is done
	//   here, which also sets up each 
	//   listener and orientation position.
	//   Also, be sure not to queue the buffers
	//   here. That is only done during playback.
	//
	// Returns:   	  bool		true or false if an error occurs
	// Parameters:
	//							<none>
	//

	bool AudioPlayer::InitSources() {

		// maximum allowed sources
		NUM_BUFFERS = 256; 
		playCount = 0;

		inuse.reserve(NUM_BUFFERS);
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			inuse.push_back( BUFFER_FREE );
		}

		Buffers = new ALuint[NUM_BUFFERS];
		Sources = new ALuint[NUM_BUFFERS];

		SourcesPos = new ALfloat[NUM_BUFFERS][3];
		SourcesVel = new ALfloat[NUM_BUFFERS][3];
		//SourcesOri = new ALfloat[NUM_BUFFERS][6];

		for ( int i = 0; i < (int)NUM_BUFFERS; i++) {
			for ( int j = 0; j < 3; j++ ) {
				SourcesPos[i][j] = 0.0;
				SourcesVel[i][j] = 0.0;
			}
		}

		for ( int i = 0; i < 3; i++ ) {
			ListenerPos[i] = 0.0;
			ListenerVel[i] = 0.0;
			ListenerOri[i] = 0.0;			
		}

		ListenerOri[2] = 1.0;
		ListenerOri[4] = -1.0;

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
			alSourcefv(Sources[i], AL_POSITION, SourcesPos[0]);	
			alSourcefv(Sources[i], AL_VELOCITY, SourcesVel[0]);
		}

		alListenerfv(AL_POSITION,    ListenerPos);
		alListenerfv(AL_VELOCITY,    ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);
	
		if( alGetError() != AL_NO_ERROR)
			return AL_FALSE;

		return true;
	}

	//
	// Delete()
	// Last modified: 16Sep2009
	//
	// Deletes allocated memory buffers and sources
	// deletes the OpenAL buffers and sources by
	// calling the OpenAL methods, also destorys
	// the playback device
	//
	// Returns:     <none>
	// Parameters:
	//      				<none>
	//

	void AudioPlayer::Delete() {

		delete [] Buffers;
		delete [] Sources;
		
		alDeleteBuffers(NUM_BUFFERS, &Buffers[0]);
		alDeleteSources(NUM_BUFFERS, &Sources[0]);

		Context = alcGetCurrentContext();
		Device = alcGetContextsDevice(Context);
		
		alcMakeContextCurrent(NULL);
		alcDestroyContext(Context);
		alcCloseDevice(Device);	
	}
	

	// 
	// static
	// loadWAVFromFile( filename )
	// Last modified: 25Oct2009
	//
	// A default function to load the wav
	// via filename and return its buffer
	//
	// Returns:     ALuint buffer
	// Parameters:
	//      filename    in      the name of the wav file
	//

	static ALuint loadWAVFromFile( char* filename ) {

		ALuint buffer;

		WAVBuffer::SimpleWAVHeader header;
		WAVBuffer parser;

		char* data = parser.ReadWAV( filename, &header );
		buffer = parser.CreateBufferFromWAV( data, header );

		return buffer;	
	}	
	

#endif