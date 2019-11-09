class Shoot {
	public:
		Shoot();
		bool loadMedia();
		void handleEvent(SDL_Event &e);
		void render();
		void close();
		void moveOffset();
		int offsetX, deltaX;
		bool goBackToMenu;
	private:
		Map map;
		Color blockColor;
		Color acidColor;
};

Shoot::Shoot() {
	offsetX = SCREEN_WIDTH / 2;
	deltaX = 0;
	goBackToMenu = false;
	blockColor = {25, 51, 66, 0xFF};
	acidColor = {0xFF, 0x69, 0x61, 0xFF};
	map = Map("shoot", -1, {{RECTANGULAR_BOX, 0, -60, 0, 0}}, blockColor, acidColor);
	map.setBackground({34, 82, 94, 0xFF});
}

bool Shoot::loadMedia() {
	bool success = true;
	if (!map.loadMedia()) success = false;

	return success;
}

void Shoot::handleEvent(SDL_Event &e) {
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

void Shoot::render() {
	map.move();
	map.render(offsetX);
	if (map.isGameOver) {
		map.reset();
	}
}

void Shoot::close() {

}

void Shoot::moveOffset() {
	offsetX += deltaX;
	if (offsetX <= 0) offsetX = 0;
	if (offsetX >= SCREEN_WIDTH / 2) offsetX = SCREEN_WIDTH / 2;
}