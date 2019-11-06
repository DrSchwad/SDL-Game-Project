class Quit {
	public:
		Quit();
		bool loadMedia();
		void render();
	private:
		LTexture quitTexture;
};

Quit::Quit() {}

bool Quit::loadMedia() {
	bool success = true;

	if (success) success = quitTexture.loadFromFile("sprites/quit.png");

	return success;
}

void Quit::render() {
	// Render background
	SDL_SetRenderDrawColor(gRenderer, 21, 24, 38, 0xFF);
	SDL_Rect quitBackgroundRect = {SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
	SDL_RenderFillRect(gRenderer, &quitBackgroundRect);

	// Render icon
	quitTexture.render(SCREEN_WIDTH / 2 + (SCREEN_WIDTH / 2 - quitTexture.getWidth()) / 2, (SCREEN_HEIGHT - quitTexture.getHeight()) / 2);
}