#include "SettingsOption.h"

const int SETTINGS_FONT_SIZE = 28;
const int OPTIONS_START_X = 220, OPTIONS_START_Y = 220;
const int OPTIONS_HEIGHT = 50;

enum SettingsKey {
	UP_KEY,
	LEFT_KEY,
	RIGHT_KEY,
	SHOOT_KEY,
	TOTAL_KEYS
};

class Settings {
	public:
		Settings();
		std::vector<int> getKeys();
		void setKey(SettingsKey key, SDL_Keycode keyCode);
		bool loadMedia();
		void handleEvent(SDL_Event &e);
		SettingsKey getEnteredOption();
		void resetEnteredOption();
		SettingsKey getCurrentSelectedOption();
		void render();
		void close();
		void moveOffset();
		int offsetX, deltaX;
		bool goBackToMenu;
	private:
		LTexture settingsTexture;
		SettingsOption settingsOptions[TOTAL_KEYS];
		SettingsKey currentSelectedOption;
		SettingsKey enteredOption;
		void renderSettingsBackground();
		void renderOptionHighlighter();
		void resetOptionHighlighter();
};

Settings::Settings() {
	offsetX = SCREEN_WIDTH / 2;
	deltaX = 0;
	currentSelectedOption = UP_KEY;
	enteredOption = TOTAL_KEYS;
	goBackToMenu = false;
}

std::vector<int> Settings::getKeys() {
	std::vector<int> codes(TOTAL_KEYS);

	std::ifstream iControlsFile(std::string("data/controls.txt"));
	for (int i = 0; i < TOTAL_KEYS; i++) iControlsFile >> codes[i];
	iControlsFile.close();

	return codes;
}

void Settings::setKey(SettingsKey key, SDL_Keycode keyCode) {
	std::vector<int> codes = getKeys();
	codes[key] = keyCode;

	std::ofstream oControlsFile(std::string("data/controls.txt"));
	std::string codeStr = "";
	for (int i = 0; i < TOTAL_KEYS; i++) codeStr += std::to_string(codes[i]) + " ";
	oControlsFile << codeStr;
	oControlsFile.close();

	UP_KEY_CODE = codes[UP_KEY];
	LEFT_KEY_CODE = codes[LEFT_KEY];
	RIGHT_KEY_CODE = codes[RIGHT_KEY];
	SHOOT_KEY_CODE = codes[SHOOT_KEY];
}

bool Settings::loadMedia() {
	bool success = true;

	// Open the font
	settingsFont = TTF_OpenFont( "fonts/settings font.ttf", SETTINGS_FONT_SIZE );
	if (settingsFont == NULL) {
		printf("Failed to load settings font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	if (success) {
		std::vector<int> codes = getKeys();

		success = settingsOptions[UP_KEY].setText("UP       " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[UP_KEY]))));
		success = settingsOptions[LEFT_KEY].setText("LEFT     " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[LEFT_KEY]))));
		success = settingsOptions[RIGHT_KEY].setText("RIGHT   " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[RIGHT_KEY]))));
		success = settingsOptions[SHOOT_KEY].setText("SHOOT   " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[SHOOT_KEY]))));

		UP_KEY_CODE = codes[UP_KEY];
		LEFT_KEY_CODE = codes[LEFT_KEY];
		RIGHT_KEY_CODE = codes[RIGHT_KEY];
		SHOOT_KEY_CODE = codes[SHOOT_KEY];
	}

	if (success) success = settingsTexture.loadFromFile("sprites/settings.png");

	return success;
}

void Settings::handleEvent(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		if (enteredOption != TOTAL_KEYS) {
			setKey(enteredOption, e.key.keysym.sym);
			enteredOption = TOTAL_KEYS;

			std::vector<int> codes = getKeys();
			settingsOptions[UP_KEY].setText("UP       " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[UP_KEY]))));
			settingsOptions[LEFT_KEY].setText("LEFT     " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[LEFT_KEY]))));
			settingsOptions[RIGHT_KEY].setText("RIGHT   " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[RIGHT_KEY]))));
			settingsOptions[SHOOT_KEY].setText("SHOOT   " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[SHOOT_KEY]))));
		}
		else {
			switch( e.key.keysym.sym ) {
				case SDLK_UP:
					resetOptionHighlighter();
					currentSelectedOption = static_cast<SettingsKey>((currentSelectedOption - 1 + TOTAL_KEYS) % TOTAL_KEYS);
					break;
				case SDLK_DOWN:
					resetOptionHighlighter();
					currentSelectedOption = static_cast<SettingsKey>((currentSelectedOption + 1) % TOTAL_KEYS);
					break;
				case SDLK_RETURN:
					enteredOption = currentSelectedOption;
					break;
				case SDLK_ESCAPE:
					goBackToMenu = true;
			}
		}
	}
}

SettingsKey Settings::getEnteredOption() {
	return enteredOption;
}
void Settings::resetEnteredOption() {
	enteredOption = TOTAL_KEYS;
}

SettingsKey Settings::getCurrentSelectedOption() {
	return currentSelectedOption;
}

void Settings::render() {
	renderSettingsBackground();

	settingsTexture.render(offsetX + (SCREEN_WIDTH / 2 - settingsTexture.getWidth()) / 2, (SCREEN_HEIGHT - settingsTexture.getHeight()) / 2);

	renderOptionHighlighter();

	for (int i = 0; i < TOTAL_KEYS; i++) {
		settingsOptions[i].setPosition(offsetX + SCREEN_WIDTH / 2 + OPTIONS_START_X, OPTIONS_START_Y + OPTIONS_HEIGHT * i);
		settingsOptions[i].render();
	}

	if (enteredOption != TOTAL_KEYS) settingsOptions[enteredOption].renderBorder();
}

void Settings::renderSettingsBackground() {
	SDL_SetRenderDrawColor(gRenderer, 21, 24, 38, 0xFF);
	SDL_Rect settingsBackgroundRect = {offsetX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_RenderFillRect(gRenderer, &settingsBackgroundRect);
}

void Settings::resetOptionHighlighter() {
	settingsOptions[currentSelectedOption].resetHighlighter();
}

void Settings::renderOptionHighlighter() {
	settingsOptions[currentSelectedOption].renderHighlighter();
}

void Settings::close() {
	//Free global font
	TTF_CloseFont( settingsFont );
	settingsFont = NULL;
	for (int i = 0; i < TOTAL_KEYS; i++)
		settingsOptions[i].close();
}

void Settings::moveOffset() {
	offsetX += deltaX;
	if (offsetX <= 0) offsetX = 0;
	if (offsetX >= SCREEN_WIDTH / 2) offsetX = SCREEN_WIDTH / 2;
}