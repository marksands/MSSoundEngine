////////////////////////////////////////////////////////////////////
// Mark Sands
// September, 4 2009
// Audio Player - a lightweight OpenAL toolkit for Audio playback
//
////////////////////////////////////////////////////////////////////

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <stdio.h>
#include "al.h"
#include "alc.h"
#include "MediaPlayer.h"


	// Inherited Audio Player	
	class AudioPlayer: public MediaPlayer, public WAVBuffer {

		public:
			AudioPlayer( const char* filenames[],  const int size = 1 );
			virtual ~AudioPlayer();

		protected:
			bool InitSources();
			void Delete();
	
			void CleanSources();
			ALuint GetFreeSource();
			ALuint CaptureBuffer( int fileIndex );
		
			ALCcontext *Context;
			ALCdevice *Device;
		
		private:
			//char *alBuffer;
			ALenum alFormatBuffer;
			ALsizei alFreqBuffer;
			long alBufferLen;
			//ALboolean alLoop;
			//unsigned int alSource;
			//unsigned int alSampleSet;

			ALuint NUM_BUFFERS;
			ALuint *Buffers;
			ALuint *Sources;
			
			ALfloat (*SourcesPos)[3];
			ALfloat (*SourcesVel)[3];
			
			ALfloat ListenerPos[3];
			ALfloat ListenerVel[3];
			ALfloat ListenerOri[6];
			
			char* filename;
			std::vector<char*> audioFiles;
	};

	// Constructor loads filename array of songs and data members
	AudioPlayer::AudioPlayer( char* filenames[], int size ) {

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
		void AudioPlayer::Play( int num, bool flag ) {
			
			int index = GetFreeSource();
			Sources[index] = CaptureBuffer( num );
			
			alSourcei(Sources[index], AL_LOOPING, (flag ? AL_TRUE : AL_FALSE) );
			alSourcePlay(Sources[index]);
		}
	
		// OpenAL Pause Sound
		void AudioPlayer::Pause( int num ) {
			alSourcePause(Sources[num]);
		}

		// OpenAL Stop sound
		void AudioPlayer::Stop( int num ) {
			alSourceStop(Sources[num]);
		}
	
	
	ALuint AudioPlayer::GetFreeSource() {
		
		CleanSources();
		
		for ( int i = 0; i < NUM_BUFFERS; i++ ) {
			if ( Sources[i] == NULL ) {
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
	
	void CleanSources()
	{
		for ( int i = 0; i < NUM_BUFFERS; i++ ) {
			if( alSourceState( Sources[i] != AL_PlAYING )
				Sources[i] = NULL;
		}
	}
	
	
	// initialize OpenAL and render sound
	bool AudioPlayer::InitSources() {

		NUM_BUFFERS = 32; // audioFiles.size();  arbitrary value; Can have up to 32 sounds playing at one time on screen. which is plenty!

		Buffers = new ALuint[NUM_BUFFERS];
		Sources = new ALuint[NUM_BUFFERS];
		SourcesPos = new ALfloat[NUM_BUFFERS][3];
		SourcesVel = new ALfloat[NUM_BUFFERS][3];

		for ( int i = 0; i < NUM_BUFFERS; i++) {
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

		// Variables to load into.
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;

		// Load wav data into buffers.
		alGenBuffers(NUM_BUFFERS, Buffers);

		for ( int i = 0; i < NUM_BUFFERS; i++) {
			// SimpleWAVHeader header;
			// char* data = ReadWAV( audioFiles[i], &header );
			// Buffers[i] = CreateBufferFromWav( data, header );

			Buffers[i] = NULL;
		}

		alGenSources(NUM_BUFFERS, Sources);

		for ( int i = 0; i < NUM_BUFFERS; i++ ) {
			alSourcefv(Sources[i], AL_POSITION, SourcesPos[0]);	
			alSourcefv(Sources[i], AL_VELOCITY, SourcesVel[0]);

			alSourceQueueBuffers( Sources[i], 1, &Buffers[i] );
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
		
		delete [] Buffers;
		delete [] Sources;
		
		alDeleteBuffers(NUM_BUFFERS, &Buffers[0]);
		alDeleteSources(NUM_BUFFERS, &Sources[0]);

		alSourceUnqueueBuffers( alSource, 1, &alSampleSet);		
		alDeleteSources(1, &alSource);
		alDeleteBuffers(1, &alSampleSet);

		Context = alcGetCurrentContext();
		Device = alcGetContextsDevice(Context);
		
		alcMakeContextCurrent(NULL);
		alcDestroyContext(Context);
		alcCloseDevice(Device);	
	}
	

#endif