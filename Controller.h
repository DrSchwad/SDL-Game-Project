#include "Menu.h"
#include "Training.h"

enum GameMode {
	MENU,
	TRAINING,
	DODGE,
	SHOOT,
	ARCADE,
	BOSS,
	SETTINGS
};

class Controller {
	public:
		Controller();
		bool loadMedia();
		void handleEvent(SDL_Event &e);
		bool isGameQuitting();
		void render();
		void close();
	private:
		Menu menu;
		Training training;
		GameMode currentGameMode;
		bool isQuitting;
		bool isWindowsMoving;
};

Controller::Controller() {
	menu = Menu();
	Training training = Training();
	currentGameMode = MENU;
	isQuitting = false;
	isWindowsMoving = false;
}

bool Controller::loadMedia() {
	bool success = true;
	if (!menu.loadMedia()) success = false;
	if (!training.loadMedia()) success = false;
	return success;
}

void Controller::handleEvent(SDL_Event &e) {
	if (isWindowsMoving) return;

	switch (currentGameMode) {
		case MENU:
			menu.handleEvent(e);

			if (menu.getEnteredOption() != TOTAL_OPTIONS) {
				isWindowsMoving = true;
				menu.deltaX = -10;
				menu.offsetX += menu.deltaX;

				switch (menu.getEnteredOption()) {
					case TRAINING_OPTION:
						training.deltaX = -10;
						training.offsetX += training.deltaX;

						currentGameMode = TRAINING;
						break;
					case DODGE_OPTION: break;
					case SHOOT_OPTION: break;
					case ARCADE_OPTION: break;
					case BOSS_OPTION: break;
					case SETTINGS_OPTION: break;
					case QUIT_OPTION:
						isQuitting = true;
						break;
					case TOTAL_OPTIONS: break;
				}

				menu.resetEnteredOption();
			}
			break;
		case TRAINING:
			training.handleEvent(e);

			if (training.goBackToMenu) {
				isWindowsMoving = true;
				menu.deltaX = 10;
				menu.moveOffset();

				training.deltaX = 10;
				training.moveOffset();
			}
			break;
		case DODGE: break;
		case SHOOT: break;
		case ARCADE: break;
		case BOSS: break;
		case SETTINGS: break;
	}
}

bool Controller::isGameQuitting() {
	return isQuitting;
}

void Controller::render() {
	if (isWindowsMoving) {
		menu.moveOffset();
		menu.render();
	}

	switch (currentGameMode) {
		case MENU:
			menu.render();
			switch (menu.getCurrentSelectedOption()) {
				case TRAINING_OPTION:
					training.render();
					break;
				case DODGE_OPTION: break;
				case SHOOT_OPTION: break;
				case ARCADE_OPTION: break;
				case BOSS_OPTION: break;
				case SETTINGS_OPTION: break;
				case QUIT_OPTION: break;
				case TOTAL_OPTIONS: break;
			}
			break;
		case TRAINING:
			training.render();

			if (isWindowsMoving) {
				training.moveOffset();

				if ((menu.offsetX == -SCREEN_WIDTH / 2 || menu.offsetX == 0) &&
					(training.offsetX == 0 || training.offsetX == SCREEN_WIDTH / 2)) {
					isWindowsMoving = false;
					if (training.goBackToMenu) {
						currentGameMode = MENU;
						training.goBackToMenu = false;
					}
				}
			}
			break;
		case DODGE: break;
		case SHOOT: break;
		case ARCADE: break;
		case BOSS: break;
		case SETTINGS: break;
	}
}

void Controller::close() {
	menu.close();
	training.close();
}