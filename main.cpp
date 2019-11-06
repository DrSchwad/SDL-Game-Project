#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "InitializeRenderer.h"
#include "LTexture.h"
#include "InitializeAssets.h"
#include "Controller.h"

int main( int argc, char* asrgs[] ) {
	freopen("logs.txt", "w", stdout);

	// Start up SDL and create windows
	if(!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		Controller controller;

		// Load media
		if(!loadMedia() || !controller.loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			// Main loop flag
			bool quit = false;

			// Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT ) {
						quit = true;
					}
					else {
						controller.handleEvent(e);
						quit = controller.isGameQuitting();
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				controller.render();

				//Update screen
				SDL_RenderPresent( gRenderer );
			}

			controller.close();
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}