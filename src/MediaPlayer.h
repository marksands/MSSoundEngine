//***************************************************************************//
// Copyright (c) 2009 Mark Sands. All rights reserved.
//
// Special thanks to Gorax on http://www.gamedev.net
//

#ifndef MEDIA_PlAYER_H
#define MEDIA_PLAYER_H

#ifdef _WIN32
  #include "al.h"
  #include "alc.h"
#else
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#endif

//***************************************************************************//
// MediaPlayer
//	Abstract class holding common user instantiated media methods.
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
//	generic class to read in a wav file and and return the wav buffer
//

class WAVBuffer {

	public:
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

		virtual ~WAVBuffer() { }
		
		char* ReadWAV( char* filename, SimpleWAVHeader* header );
		ALuint CreateBufferFromWav( char* data, SimpleWAVHeader header );
};
	
	

//
// ReadWAV( filename, header )
// Last modified: 04Sep2009
//
// the WAVBuffer method that reads the wav into the header
// and returns the buffer data from the wav file
//
// Returns:     char* buffer (data)
// Parameters:
//      filename    in      the name of the wav file
//      header      in      the wav header struct
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


//
// CreateBufferFromWav( data, header )
// Last modified: 04Sep2009
//
// the WAVBuffer method that generates and creates
// an audio buffer from the wav data
//
// Returns:     ALuint buffer
// Parameters:
//      data    in   the data buffer from the wav file
//      header  in   the wav header struct
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




//
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

ALuint loadWAVFromFile( char* filename ) {
	
	ALuint buffer;
	
	WAVBuffer::SimpleWAVHeader header;
	WAVBuffer parser;
	
	char* data = parser.ReadWAV( filename, &header );
	buffer = parser.CreateBufferFromWAV( data, header );
	
	return buffer;	
}

#endif
