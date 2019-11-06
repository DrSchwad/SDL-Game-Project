class Dodge {
	public:
		Dodge();
		bool loadMedia();
		void changeLevel(int delta);
		int getLevel();
		void handleEvent(SDL_Event &e);
		void render();
		void close();
		void moveOffset();
		int offsetX, deltaX;
		bool goBackToMenu;
	private:
		Map map;
		int level;
};

Dodge::Dodge() {
	offsetX = SCREEN_WIDTH / 2;
	deltaX = 0;
	goBackToMenu = false;
	level = 0;
	map = Map("dodge", level, {{RECTANGULAR_BOX, 0, -60, 0, 0}}, {25, 51, 66, 0xFF}, {139, 224, 72, 0xFF});
	map.setBackground({34, 82, 94, 0xFF});
}

bool Dodge::loadMedia() {
	bool success = true;
	if (!map.loadMedia()) success = false;

	return success;
}

void Dodge::changeLevel(int delta) {
	level = (level + delta + TOTAL_DODGE_LEVELS) % TOTAL_DODGE_LEVELS;
	map = Map("dodge", level, {{RECTANGULAR_BOX, 0, -60, 0, 0}}, {25, 51, 66, 0xFF}, {139, 224, 72, 0xFF});
	map.setBackground({34, 82, 94, 0xFF});
	map.loadMedia();
}

int Dodge::getLevel() {
	return level;
}

void Dodge::handleEvent(SDL_Event &e) {
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

void Dodge::render() {
	map.move();
	map.render(offsetX);
	if (map.isLevelFinished) {
		SDL_Delay(1000);
		changeLevel(1);
	}

	if (map.isGameOver) {
		map.reset();
	}
}

void Dodge::close() {

}

void Dodge::moveOffset() {
	offsetX += deltaX;
	if (offsetX <= 0) offsetX = 0;
	if (offsetX >= SCREEN_WIDTH / 2) offsetX = SCREEN_WIDTH / 2;
}