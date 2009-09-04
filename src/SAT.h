////////////////////////////////////////////////////
// Mark Sands
// September, 4 2009
// Simple Audio Toolkit
//
////////////////////////////////////////////////////

#ifndef SAT_H
#define SAT_H

#include "al.h"
#include "alc.h"
#include "alut.h"

namespace OpenALManager {
	class SAT {
		public:
			SAT( char* filename = "NA" );
			virtual ~SAT();
			void PlaySound( bool flag = AL_TRUE);
			void PauseSound();
			void StopSound();
		private:
			bool InitSound();
			void DeleteSound();	
			
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
	};

	// Constructor loads filename and data members
	SAT::SAT( char* filename ) {
		SAT::filename = filename;

		if ( filename == "NA" )
			std::cerr << "Please specify a filename!";
			
		if ( !InitSound() )
			std::cerr < "Failed to initialize OpenAL!";
	}

	// Destroy members for reinit
	SAT::~SAT() {
		DeleteSound();
	}

	// OpenAL Play Sound
	void SAT::PlaySound( bool flag ) {
		alSourcei(alSource, AL_LOOPING, (flag ? AL_TRUE : AL_FALSE) );
		alSourcePlay(alSource);
	}
	
	// OpenAL Pause Sound
	void SAT::PauseSound() {
		alSourcePause(alSource);
	}
	
	// OpenAL Stop sound
	void SAT::StopSound() {
		alSourceStop(alSource);
	}
	
	// initialize OpenAL members	
	bool SAT::InitSound() {
		Device = alcOpenDevice( (ALCchar*)"DirectSound3D" );
		if (Device == NULL)
			return false;

		Context = alcCreateContext(Device,NULL);
		alcMakeContextCurrent(Context);
		alGetError();

		// loads WAV from private member filename
		alutLoadWAVFile( filename, &alFormatBuffer, (void**)&alBuffer, (ALsizei*)&alBufferLen, &alFreqBuffer, &alLoop );

		alGenSources(1, &alSource);
		alGenBuffers(1, &alSampleSet);
		alBufferData(alSampleSet, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		alSourcei(alSource, AL_BUFFER, alSampleSet);

		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);

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
	void SAT::DeleteSound() {
		alDeleteSources(1, &alSource);
		alDeleteBuffers(1, &alSampleSet);

		Context = alcGetCurrentContext();
		Device = alcGetContextsDevice(Context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(Context);
		alcCloseDevice(Device);	
	}
}

#endif