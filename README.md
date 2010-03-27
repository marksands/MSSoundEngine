# balt

Basic OpenAL Toolkit - a simple audio toolkit used for executing trivial OpenAL function calls for
basic sound rendering. A work in progress for CS 482 - Computer Graphics. Adding sound for pizazz
and brownie points.

## Usage

Add files to a char* array.
	
	char* files[3] = { "gun1.wav", "gun2.wav", "bomb.wav", "theme.wav" };
	
Instantiate a new AudioPlayer object. The constructor takes the char* array of files as 
its parameter along with the size of the array.
	
	MediaPlayer *Player = new AudioPlayer( files, 4 );
	
Play the theme song. Optionally set the loop flag to true.
	
	Player->Play( THEME, true );
	
Play a sound effect in sync with the theme and such; or pause all sounds.
	
	Player->Play( EXPLOSION );
	
		// ...
	
	Player->Pause();
	
Feel free to alter the sound, too.

		// Set volume to half
	Player->SetVolume( 0.5 );
	
You can even stop the player.

	Player->Stop();
	
Check out the sample code in the examples directory.

## Build Process

On OS X, you just specify `-framework OpenAL`

example: `g++ main.cc -framework OpenAL && ./a.out`

In MSVC++, add everything to the project, including all source files, all sound files,
and the openAL lib files. Unlike glut, for example, you can't simply leave them in the 
directory, you have to manually add them to project.
