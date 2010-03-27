#include "../src/balt.hpp"

const int THEME = 0;

int main (int argc, char const *argv[])
{
	char* files[1] = { "sample.wav" };
	
	MediaPlayer *Player = new AudioPlayer( files );
	
	Player->Play( THEME, true );
			
	// ...
						
	delete Player;

	return 0;
}