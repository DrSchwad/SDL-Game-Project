// Scene texture
// LTexture gArrowTexture;

// Loads media
bool loadMedia() {
	// Loading success flag
	bool success = true;

	// Load arrow
	// if(!gArrowTexture.loadFromFile("arrow.png")) {
	// 	printf("Failed to load arrow texture!\n");
	// 	success = false;
	// }

	return success;
}

// Frees media and shuts down SDL
void close() {
	// Free loaded images
	// gArrowTexture.free();

	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

