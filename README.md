# Balto

### What is Balto?

Balto is a lightweight OpenAL toolkit for Audio playback designed to encapsulate the low level audio handling and provide the user with a nice, easy to use high level application programming interface for sound rendering in their applications.

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

On OS X, you just specify the `-framework OpenAL` option

	g++ main.cc -framework OpenAL && ./a.out

On Windows, you should be fine as long as you include all source, sound, and lib files
to your project. This is currently untested on GNU/linux, but I have faith that it works. :)

## Author

- [Mark Sands](http://www.github.com/marksands) 
- <marksands07@gmail.com> 
- <http://www.twitter.com/marksands>

## License

See [License](http://github.com/marksands/Balto/blob/master/LICENSE)