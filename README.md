Audio-Player
------------

A Simple Audio Toolkit used for executing trivial OpenAL function calls for basic sound rendering.
A work in progress for CS 482 - Computer Graphics. (Adding sound for pizazz and brownie points)


Usage
-----

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


Build Process
-------------

	On OS X, you just specify '-framework OpenAL'
	g++ main.cpp -framework OpenGL -framework GLUT -framework Cocoa -framework OpenAL && ./a.out
	
	If you ran into problems like I did, you might have a bad OpenAL framework hiding in your Library. Just
	go to /Library/Frameworks/ and rename OpenAL.framework to something like OpenAL-bad.framework and it
	will find the right framework.
	
	In MS VC2008, add the source files to the project, all sound files, and even the openAL lib files.
	You can't simply leave them in the directory, you have to manually add them to project.
	It should work with no problems.
	
	Hope this helps :)
	
	Mark