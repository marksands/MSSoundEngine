//***************************************************************************//
// Copyright (c) 2009 Mark Sands. All rights reserved.
// September, 4 2009
// Audio Player - a lightweight OpenAL toolkit for Audio playback
//
//***************************************************************************//

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <iostream>
#include <vector>
#include "al.h"
#include "alc.h"
#include "MediaPlayer.h"

//***************************************************************************//
// AduioPlayer
//	Main class for reading user audio files and producing the audio output
//  provides inherited member fuctions from MediaPlayer
// 
	class AudioPlayer: public MediaPlayer, public WAVBuffer {

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
			ALuint CaptureBuffer( int fileIndex );
		
			ALCcontext *Context;
			ALCdevice *Device;
		
		private:
			ALuint NUM_BUFFERS;
			ALuint *Buffers;
			ALuint *Sources;
			
			ALfloat (*SourcesPos)[3];
			ALfloat (*SourcesVel)[3];
			
			ALfloat ListenerPos[3];
			ALfloat ListenerVel[3];
			ALfloat ListenerOri[6];
			
			int* inuse;
			char* filename;
			std::vector<char*> audioFiles;
	};

	// Constructor loads filename array of songs and data members
	AudioPlayer::AudioPlayer(char *filenames[], const int size) {

		for( int i = 0; i < size; i++ )
			audioFiles.push_back( filenames[i] );
			
		if ( !InitSources() )
			std::cerr << "Failed to initialize OpenAL!";
	}

	// Destroy members for reinit
	AudioPlayer::~AudioPlayer() {
		Delete();
	}

		// OpenAL Play Sound
		void AudioPlayer::Play( int index, bool looping ) {
			
			CleanSources();

			int num = GetFreeSource();
			ALuint buffer = CaptureBuffer( index );

			alSourceQueueBuffers( Sources[num], 1, &buffer );

			alSourcei(Sources[num], AL_LOOPING, (looping ? AL_TRUE : AL_FALSE) );
			alSourcePlay(Sources[num]);
		}
	

		// you almost cant call these functions... very ambiguous?
		//  especially with a dynamic sources container?

		// OpenAL Pause Sound
		void AudioPlayer::Pause( int index ) {
			//alSourcePause(Sources[index]);
		}

		// OpenAL Stop sound
		void AudioPlayer::Stop( int index ) {
			//alSourceStop(Sources[index]);
		}
		
		void AudioPlayer::SetVolume( const int index,  const float volume) {
			for ( int i = 0; i < (int)NUM_BUFFERS; i++ )
				alSourcei( Sources[i], AL_GAIN, volume );
		}
	
	
	ALuint AudioPlayer::GetFreeSource() {
		
		ALenum state;
		
		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			alGetSourcei( Sources[i], AL_SOURCE_STATE, &state );
			if( state != AL_PLAYING ) {
   				inuse[i] = 1;
				std::cout << "Source pos: " << i << std::endl;
				return i;
			}
		}
		
		return -1;
	}
	
	ALuint AudioPlayer::CaptureBuffer( int fileIndex ) {
		
		ALuint Buffer;
		SimpleWAVHeader header;
		
		char* data = ReadWAV( audioFiles[fileIndex], &header );
		Buffer = CreateBufferFromWav( data, header );

		return ( Buffer );
	}
	
	// need to unqueue unplayed buffers here :)
	void AudioPlayer::CleanSources()
	{
		ALenum state;

		for ( int i = 0; i < (int)NUM_BUFFERS; i++ ) {
			if ( inuse[i] == 1 ) {
				alGetSourcei( Sources[i], AL_SOURCE_STATE, &state );
				if ( state != AL_PLAYING ) {
					alSourceStop( Sources[i] );
					alSourcei( Sources[i], AL_BUFFER, 0 );
					inuse[i] = 0;
				}
			}
		}
	}
	
	
	// initialize OpenAL and render sound
	bool AudioPlayer::InitSources() {

		NUM_BUFFERS = 32; 
		inuse = new int[NUM_BUFFERS];

		Buffers = new ALuint[NUM_BUFFERS];
		Sources = new ALuint[NUM_BUFFERS];

		SourcesPos = new ALfloat[NUM_BUFFERS][3];
		SourcesVel = new ALfloat[NUM_BUFFERS][3];

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

		// Load wav data into buffers.
		alGenBuffers(NUM_BUFFERS, Buffers);

		// set our empty buffers to NULL
		for ( int i = 0; i < (int)NUM_BUFFERS; i++) {
			Buffers[i] = NULL;
		}

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

	// OpenAL Delete sound	
	void AudioPlayer::Delete() {
		
		delete [] inuse;

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
	

#endif