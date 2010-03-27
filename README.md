# balt

Basic OpenAL Toolkit - a simple audio toolkit used for executing trivial OpenAL function calls for
basic sound rendering. A work in progress for CS 482 - Computer Graphics. Adding sound for pizazz
and brownie points.

## Usage

	#define GUN_1 0
	#define GUN_2 1
 	#define EXPLOSION 2
	#define THEME 3
	
	char* files[3] = { "gun1.wav", "gun2.wav", "bomb.wav", "theme.wav" };
	
	MediaPlayer *Player = new AudioPlayer( files, 4 );
	
	Player->Play( THEME, true ); // loop song
		
	Player->Play( GUN_1 );
	Player->Play( GUN_2 );
	Player->Play( EXPLOSION );
	
	// ...
	
	delete Player;


## Build Process

On OS X, you just specify `-framework OpenAL`

example: `g++ main.cc -framework OpenAL && ./a.out`

In MSVC++, add everything to the project, including all source files, all sound files,
and the openAL lib files. Unlike glut, for example, you can't simply leave them in the 
directory, you have to manually add them to project.
