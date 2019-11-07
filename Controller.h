const int TOTAL_DODGE_LEVELS = 3;

int UP_KEY_CODE, LEFT_KEY_CODE, RIGHT_KEY_CODE, SHOOT_KEY_CODE;

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

#include "CollisionArea.h"
#include "Color.h"
#include "Map.h"
#include "Menu.h"
#include "Training.h"
#include "Dodge.h"
#include "Settings.h"
#include "Leaderboard.h"
#include "Quit.h"

enum GameMode {
	MENU,
	TRAINING,
	DODGE,
	SHOOT,
	ARCADE,
	LEADERBOARD,
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
		Dodge dodge;
		Settings settings;
		Leaderboard leaderboard;
		Quit quit;
		GameMode currentGameMode;
		bool isQuitting;
		bool isWindowsMoving;
};

Controller::Controller() {
	currentGameMode = MENU;
	isQuitting = false;
	isWindowsMoving = false;
}

bool Controller::loadMedia() {
	bool success = true;
	if (!menu.loadMedia()) success = false;
	if (!training.loadMedia()) success = false;
	if (!dodge.loadMedia()) success = false;
	if (!settings.loadMedia()) success = false;
	if (!leaderboard.loadMedia()) success = false;
	if (!quit.loadMedia()) success = false;
	return success;
}

void Controller::handleEvent(SDL_Event &e) {
	if (isWindowsMoving) return;

	switch (currentGameMode) {
		case MENU:
			menu.handleEvent(e);

			if (menu.getEnteredOption() != TOTAL_OPTIONS && menu.getEnteredOption() != LEADERBOARD_OPTION) {
				isWindowsMoving = true;
				menu.deltaX = -10;
				menu.offsetX += menu.deltaX;

				switch (menu.getEnteredOption()) {
					case TRAINING_OPTION:
						training.deltaX = -10;
						training.offsetX += training.deltaX;

						currentGameMode = TRAINING;
						break;
					case DODGE_OPTION:
						dodge.deltaX = -10;
						dodge.offsetX += dodge.deltaX;

						currentGameMode = DODGE;
						break;
					case SHOOT_OPTION: break;
					case ARCADE_OPTION: break;
					case LEADERBOARD_OPTION: break;
					case SETTINGS_OPTION:
						settings.deltaX = -10;
						settings.offsetX += settings.deltaX;

						currentGameMode = SETTINGS;
						break;
					case QUIT_OPTION:
						isQuitting = true;
						break;
					case TOTAL_OPTIONS: break;
				}

				menu.resetEnteredOption();
			}
			else if (menu.levelDelta != 0) {
				if (menu.getCurrentSelectedOption() == DODGE_OPTION) {
					dodge.changeLevel(menu.levelDelta);
					menu.levelDelta = 0;
					break;
				}
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
		case DODGE:
			dodge.handleEvent(e);

			if (dodge.goBackToMenu) {
				isWindowsMoving = true;
				menu.deltaX = 10;
				menu.moveOffset();

				dodge.deltaX = 10;
				dodge.moveOffset();
			}
			break;
		case SHOOT: break;
		case ARCADE: break;
		case LEADERBOARD: break;
		case SETTINGS:
			settings.handleEvent(e);

			if (settings.goBackToMenu) {
				isWindowsMoving = true;
				menu.deltaX = 10;
				menu.moveOffset();

				settings.deltaX = 10;
				settings.moveOffset();
			}
			break;
	}
}

bool Controller::isGameQuitting() {
	return isQuitting;
}

void Controller::render() {
	switch (currentGameMode) {
		case MENU:
			menu.render();
			switch (menu.getCurrentSelectedOption()) {
				case TRAINING_OPTION:
					training.render();
					break;
				case DODGE_OPTION:
					dodge.render();
					menu.renderLevelIndicator(dodge.getLevel());
					break;
				case SHOOT_OPTION: break;
				case ARCADE_OPTION: break;
				case LEADERBOARD_OPTION:
					leaderboard.render();
					break;
				case SETTINGS_OPTION:
					settings.render();
					break;
				case QUIT_OPTION:
					quit.render();
					break;
				case TOTAL_OPTIONS: break;
			}
			break;
		case TRAINING:
			if (isWindowsMoving) training.moveOffset();
			training.render();

			if (isWindowsMoving) {
				menu.moveOffset();
				menu.render();

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
		case DODGE:
			if (isWindowsMoving) dodge.moveOffset();
			dodge.render();

			if (isWindowsMoving) {
				menu.moveOffset();
				menu.render();

				if ((menu.offsetX == -SCREEN_WIDTH / 2 || menu.offsetX == 0) &&
					(dodge.offsetX == 0 || dodge.offsetX == SCREEN_WIDTH / 2)) {
					isWindowsMoving = false;
					if (dodge.goBackToMenu) {
						currentGameMode = MENU;
						dodge.goBackToMenu = false;
					}
				}
			}

			break;
		case SHOOT: break;
		case ARCADE: break;
		case LEADERBOARD: break;
		case SETTINGS:
			if (isWindowsMoving) settings.moveOffset();
			settings.render();

			if (isWindowsMoving) {
				menu.moveOffset();
				menu.render();

				if ((menu.offsetX == -SCREEN_WIDTH / 2 || menu.offsetX == 0) &&
					(settings.offsetX == 0 || settings.offsetX == SCREEN_WIDTH / 2)) {
					isWindowsMoving = false;
					if (settings.goBackToMenu) {
						currentGameMode = MENU;
						settings.goBackToMenu = false;
					}
				}
			}

			break;
	}
}

void Controller::close() {
	menu.close();
	training.close();
	dodge.close();
	settings.close();
}