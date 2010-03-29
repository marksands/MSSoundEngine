/***
 * Copyright (c) 2009, 2010 Mark Sands. All rights reserved.
 *
 * This sample file demonstrates very basic usage of Balto.
 * If you're using this to develop an audio player, I suggest
 * you extend this library or use raw OpenAL yourself ;). 
 * However, I find this library to be extremely helpful when
 * creating OpenGL games that require basic sound rendering
 * which relieves a lot of the programming and makes it a joy
 * to use.
 *
 ***/

#include "../src/Balto.hpp"
#include <unistd.h>

const int THEME = 0;

int main (int argc, char const *argv[])
{
	char* files[1] = { "sample.wav" };
	
	MediaPlayer *Player = new AudioPlayer( files );	
	Player->Load();
	
	Player->Play( THEME, true );

	sleep(5);
	
	for ( int i = 0; i < 10; i++)
		Player->Play( THEME, true );
		
	sleep(5);
	
	for ( int i = 0; i < 10; i++)
		Player->Play( THEME, true );

				
		// ... let the song play out
	sleep(30);

	delete Player;
	return 0;
}