class Training {
	public:
		Training();
		bool loadMedia();
		void handleEvent(SDL_Event &e);
		void render();
		void close();
		void moveOffset();
		int offsetX, deltaX;
		bool goBackToMenu;
	private:
		Map map;
};

Training::Training() {
	offsetX = SCREEN_WIDTH / 2;
	deltaX = 0;
	goBackToMenu = false;
	map = Map("training", -1, {{RECTANGULAR_BOX, 0, -60, 0, 0}}, {0xD6, 0xE6, 0xF1, 0xFF});
	map.setBackground();
}

bool Training::loadMedia() {
	bool success = true;
	if (!map.loadMedia()) success = false;

	return success;
}

void Training::handleEvent(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		switch( e.key.keysym.sym ) {
			case SDLK_ESCAPE:
				goBackToMenu = true;
			default:
				map.handleEvent(e);
		}
	}

	if (e.type == SDL_KEYUP) {
		switch( e.key.keysym.sym ) {
			case SDLK_ESCAPE:
				break;
			default:
				map.handleEvent(e);
		}
	}
}

void Training::render() {
	map.move();
	map.render(offsetX);
}

void Training::close() {

}

void Training::moveOffset() {
	offsetX += deltaX;
	if (offsetX <= 0) offsetX = 0;
	if (offsetX >= SCREEN_WIDTH / 2) offsetX = SCREEN_WIDTH / 2;
}