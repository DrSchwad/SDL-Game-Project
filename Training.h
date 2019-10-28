#include "Hero.h"

LTexture groundMiddle;
LTexture trainingBackground;

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
		Hero hero;
};

Training::Training() {
	offsetX = SCREEN_WIDTH / 2;
	deltaX = 0;
	goBackToMenu = false;
	hero = Hero();
}

bool Training::loadMedia() {
	bool success = true;
	if (!groundMiddle.loadFromFile("sprites/ground middle.png")) success = false;
	if (!trainingBackground.loadFromFile("sprites/Training Background.png")) success = false;
	if (!hero.loadMedia()) success = false;
	return success;
}

void Training::handleEvent(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		switch( e.key.keysym.sym ) {
			case SDLK_ESCAPE:
				goBackToMenu = true;
		}
	}
}

void Training::render() {
	// Render background
	trainingBackground.render(offsetX, SCREEN_HEIGHT - trainingBackground.getHeight());
	trainingBackground.render(offsetX + trainingBackground.getWidth(), SCREEN_HEIGHT - trainingBackground.getHeight());

	// // Render grass
	// int atX = offsetX;
	// while (atX < SCREEN_WIDTH + offsetX) {
	// 	groundMiddle.render(atX, SCREEN_HEIGHT - groundMiddle.getHeight());
	// 	atX += groundMiddle.getWidth();
	// }

	// Render hero
	hero.renderShadow();
	hero.render(offsetX, 60);
}

void Training::close() {

}

void Training::moveOffset() {
	offsetX += deltaX;
	if (offsetX <= 0) offsetX = 0;
	if (offsetX >= SCREEN_WIDTH / 2) offsetX = SCREEN_WIDTH / 2;
}