#include "../src/Balto.hpp"
#include <unistd.h>

const int THEME = 0;

int main (int argc, char const *argv[])
{
	char* files[1] = { "sample.wav" };
	
	MediaPlayer *Player = new AudioPlayer( files );	
	Player->Load();
	
	Player->Play( THEME, true );
				
		// ... let the song play out
	sleep(30);

	delete Player;
	return 0;
}