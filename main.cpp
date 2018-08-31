#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "tongits/includes/CTongIts.h"

int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	CTongIts	Game;

	//Options
	bool Aggressive = false;
//	unsigned int Video = SDL_SWSURFACE|SDL_HWPALETTE;
	unsigned int Video = 0;

	//check for command-line arguments
	while ( argc > 1 ) {
	
		if ( strcmp(argv[1], "--aggressive") == 0 ) {

			Aggressive = true;
			argv++;
			argc--;
		} else
		
  		if ( strcmp(argv[1], "--fullscreen") == 0 ) {

			Video |= SDL_FULLSCREEN;
			argv++;
			argc--;
		} else break;
	}

	if( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {

		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	} atexit(SDL_Quit);

	screen = SDL_SetVideoMode(640, 480,  0, Video);	//'0' lets the game 'adapt' to the color depth

	if(screen == NULL) {

		printf("Unable to set 640x480 video: %s\n", SDL_GetError());
		exit(1);
	}


	Game.SetAggresive(Aggressive);
	Game.Initialize(screen);
	int done = Game.HandleIntro();	//show the intro screen

	SDL_Event event;

	SDL_EventState (SDL_ACTIVEEVENT, SDL_IGNORE);

	//the game loop
	while(done == 0)
	{
		while(SDL_PollEvent(&event))
		{
			Game.HandleEvents(event, &done);
			if(event.type == SDL_QUIT) return 0;
		}
		SDL_Delay(SDL_TIMESLICE);
	}
	
	return 0;
}

