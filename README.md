# Balto

### What is Balto?

Balto is a lightweight OpenAL toolkit for Audio playback designed to encapsulate the low level audio handling and provide the user with a nice, easy to use high level application programming interface for sound rendering in their applications.

Currently Balto is available in as a C++ library and as an Objective-C library, which makes it super easy to use on separate platforms like Windows, Mac, or iPhone. The API is the same across platforms, just check out the sample code to get an idea of how to get things up and running.

Balto currently only renders WAV formatted files extended from RIFF.

## Usage

### C++

Add files to a char* array.
	
	char* files[4] = { "gun1.wav", "gun2.wav", "bomb.wav", "theme.wav" };
	
Instantiate a new Balto object. The constructor takes the char* array of files as 
its parameter along with the size of the array.
	
	MediaPlayer *Player = new Balto( files, 4 );
	
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
	
### Objective-C

Add files to a `NSMutableArray`.
	
	NSString *filePath = [[NSBundle mainBundle] pathForResource:@"sample" ofType:@"wav"];

	NSMutableArray *filenames = [[NSMutableArray alloc] initWithCapacity:1];
	[filenames addObject:filePath];

Instantiate a new Balto object, initializing with the array of filenames.

	Balto *Player = [[Balto alloc] initWithFiles:filenames];

Play the theme song optionally setting the loop flag to true. And when we're finished, we stop
the player and  do some cleanup.

	[Player Play:THEME andLooping:YES];
	
	// ...	
		
	[Player Stop];
	[Player release];
	[filenames release];
		
Don't forget to check out the sample code in the examples directory.

## Build Process

On OS X, you just specify the `-framework OpenAL` option.

	g++ main.cc -framework OpenAL && ./a.out

Be sure and add the OpenAL.framework to your existing project frameworks within Xcode if
you're using the Objective-C library.

On Windows, you should be fine as long as you include all source, sound, and lib files
to your project. This is currently untested on GNU/linux, but I have faith that it works. :)

## Author

- [Mark Sands](http://www.github.com/marksands) 
- <marksands07@gmail.com> 
- <http://www.twitter.com/marksands>

## License

See [License](http://github.com/marksands/Balto/blob/master/LICENSE)