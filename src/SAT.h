////////////////////////////////////////////////////
// Mark Sands
// September, 4 2009
// Simple Audio Toolkit
//
////////////////////////////////////////////////////

#ifndef SAT_H
#define SAT_H

#include <stdio.h>
#include <stdlib.h>
#include "al.h"
#include "alc.h"
#include "alut.h"

namespace OpenALManager {
	
	typedef struct {
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
	
	class SAT {
		public:
			SAT( char* filename = "NA" );
			SAT( char* filenames[] );
			virtual ~SAT();
			void Play( bool flag = AL_TRUE);
			void Pause();
			void Stop();
		private:
			bool Init();
			void Delete();
			
			char* ReadWAV( char* filename, SimpleWAVHeader* header );
			ALuint CreateBufferFromWav( char* data, SimpleWAVHeader header );	
			
			ALCcontext *Context;
			ALCdevice *Device;

			char *alBuffer;
			ALenum alFormatBuffer;
			ALsizei alFreqBuffer;
			long alBufferLen;
			ALboolean alLoop;
			unsigned int alSource;
			unsigned int alSampleSet;
			
			char* filename;
			char* filenames[];
	};

	// Constructor loads filename and data members
	SAT::SAT( char* filename ) {
		SAT::filename = filename;

		if ( filename == "NA" )
			std::cerr << "Please specify a filename!";
			
		if ( !Init() )
			std::cerr << "Failed to initialize OpenAL!";
	}

	// Constructor loads filename array of songs and data members
	SAT::SAT( char* filenames[] ) {
		//SAT::filenames = filename;
			
		if ( !Init() )
			std::cerr << "Failed to initialize OpenAL!";
	}

	// Destroy members for reinit
	SAT::~SAT() {
		Delete();
	}

	// OpenAL Play Sound
	void SAT::Play( bool flag ) {
		alSourcei(alSource, AL_LOOPING, (flag ? AL_TRUE : AL_FALSE) );
		alSourcePlay(alSource);
	}
	
	// OpenAL Pause Sound
	void SAT::Pause() {
		alSourcePause(alSource);
	}
	
	// OpenAL Stop sound
	void SAT::Stop() {
		alSourceStop(alSource);
	}
	
	// initialize OpenAL and render sound
	bool SAT::Init() {
		Device = alcOpenDevice( (ALCchar*)"DirectSound3D" );
		if (Device == NULL)
			return false;

		Context = alcCreateContext(Device,NULL);
		alcMakeContextCurrent(Context);
		alGetError();
		
		// replaces the deprecated alutLoadWAVFromFile() function --
		SimpleWAVHeader header;
		char* data = ReadWAV( filename, &header );
		ALuint buffer = CreateBufferFromWav( data, header );
		
		alGenSources(1, &alSource);
		alSourceQueueBuffers(alSource,1,&buffer);
		// --

		ALfloat xposition = 0.0, yposition = 0.0, zposition = 0.0,
				xvelocity = 0.0, yvelocity = 0.0, zvelocity = 0.0;

		alSource3f(alSource, AL_POSITION, xposition, yposition, zposition);
		alSource3f(alSource, AL_VELOCITY, xvelocity, yvelocity, zvelocity);

		ALfloat listenerx, listenery, listenerz;

		listenerx = 10.0f;
		listenery = 0.0f;
		listenerz = 5.0f;

		ALfloat vec[6] = { 10.0, 0.0, 5.0, 0.0, 0.0, 0.0 };

		alListener3f(AL_POSITION, listenerx, listenery, listenerz);
		alListenerfv(AL_ORIENTATION, vec);
		
		return true;
	}

	// OpenAL Delete sound	
	void SAT::Delete() {
		
		alSourceUnqueueBuffers( alSource, 1, &alSampleSet);		
		alDeleteSources(1, &alSource);
		alDeleteBuffers(1, &alSampleSet);

		Context = alcGetCurrentContext();
		Device = alcGetContextsDevice(Context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(Context);
		alcCloseDevice(Device);	
	}
	
	// Reads in the user's wav file into data
	//  - Function borrowed and refactored from Gorax on gamedev.net
	char* SAT::ReadWAV( char* filename, SimpleWAVHeader* header ) {

		char* buffer = 0;
		FILE* file = fopen(filename,"rb");

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

		return 0;
	}

	// Creates a WAV buffer from the wav data
	//  - Function borrowed and refactored from Gorax on gamedev.net
	ALuint SAT::CreateBufferFromWav( char* data, SimpleWAVHeader header ) {

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

		return buffer;
	}
	
	
}

#endif