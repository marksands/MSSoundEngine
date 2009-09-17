//***************************************************************************//
// Copyright (c) 2009 Mark Sands. All rights reserved.
//
//
#ifndef MEDIA_PlAYER_H
#define MEDIA_PLAYER_H

#include <stdio.h>
#include "al.h"
#include "alc.h"

//***************************************************************************//
// MediaPlayer
//	Abstract class holding common user instantiated media input methods.
// 

class MediaPlayer {
	public:
		virtual ~MediaPlayer() { }
	
		virtual void Play( const int index = 1, bool looping = false) = 0;
		virtual void Pause( const int index = 1) = 0;
		virtual void Stop( const int index = 1) = 0;
		virtual void SetVolume( const int index = 1,  const float volume = 1.0f) = 0;		
};

//***************************************************************************//
// WAVBuffer
//	Extracts common WAV files into buffers for OpenAL.
// 
//  Shout out to Gorax on http://www.gamedev.net for his input
//    on reading and creating the WAV buffer
//

class WAVBuffer {

	protected:
		// WAVheader
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

	public:
		virtual ~WAVBuffer() { }
		
		char* ReadWAV( char* filename, SimpleWAVHeader* header );
		ALuint CreateBufferFromWav( char* data, SimpleWAVHeader header );
};
	
	
//***************************************************************************//
// char* WAVBuffer::ReadWav( char* filename, SimpleWAVHeader* header ) { ... }
//
// Reads in the user's wav file into a buffer using a SimpleWAVheader 
//  struct to read the WAV header and returns the buffer data
//

char* WAVBuffer::ReadWAV( char* filename, SimpleWAVHeader* header ) {

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

//************************************************************************************//
// ALuint WAVBuffer::CreateBufferFromWav( char* data, SimpleWAVHeader header ) { ... }
//
// Creates an audio buffer from the WAV data specified by the data param
//

ALuint WAVBuffer::CreateBufferFromWav( char* data, SimpleWAVHeader header ) {

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

#endif