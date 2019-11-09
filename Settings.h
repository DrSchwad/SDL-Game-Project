#include "SettingsOption.h"
#include "SettingsHeader.h"
#include "SettingsTextField.h"

const int SETTINGS_HEADER_FONT_SIZE = 20;
const int OPTIONS_FONT_SIZE = 28;
const int OPTIONS_START_X = 220, OPTIONS_START_Y = 250;
const int OPTIONS_HEIGHT = 50;

enum SettingsKey {
	NAME,
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
		SettingsHeader settingsHeader[2];
		SettingsTextField settingsTextField;
		SettingsOption settingsOptions[TOTAL_KEYS];
		SettingsKey currentSelectedOption;
		SettingsKey enteredOption;
		void renderSettingsBackground();
		void renderOptionHighlighter();
		void resetOptionHighlighter();
		void renderThinBorder();
};

Settings::Settings() {
	offsetX = SCREEN_WIDTH / 2;
	deltaX = 0;
	currentSelectedOption = UP_KEY;
	enteredOption = TOTAL_KEYS;
	goBackToMenu = false;

	settingsTextField.setBackgroundColor({38, 44, 58});
	settingsTextField.setTextColor({0xFF, 0xFF, 0xFF});
	settingsTextField.setHintTextColor({0xCC, 0xCC, 0xCC});
	settingsTextField.setHintText("Enter your name");
}

std::vector<int> Settings::getKeys() {
	std::vector<int> codes(TOTAL_KEYS);

	std::ifstream iControlsFile(std::string("data/controls.txt"));
	for (int i = 1; i < TOTAL_KEYS; i++) iControlsFile >> codes[i];
	iControlsFile.close();

	return codes;
}

void Settings::setKey(SettingsKey key, SDL_Keycode keyCode) {
	std::vector<int> codes = getKeys();
	codes.insert(codes.begin(), 0);
	codes[key] = keyCode;

	std::ofstream oControlsFile(std::string("data/controls.txt"));
	std::string codeStr = "";
	for (int i = 1; i < TOTAL_KEYS; i++) codeStr += std::to_string(codes[i]) + " ";
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
	optionsFont = TTF_OpenFont( "fonts/settings font.ttf", OPTIONS_FONT_SIZE );
	if (optionsFont == NULL) {
		printf("Failed to load settings options font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	if (success) {
		settingsheaderFont = TTF_OpenFont( "fonts/settings font.ttf", SETTINGS_HEADER_FONT_SIZE );
		if (settingsheaderFont == NULL) {
			printf("Failed to load settings header font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
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

	if (success) {
		success = settingsHeader[0].setText("Player name");
		success = settingsHeader[1].setText("Controls");
	}

	if (success) success = settingsTexture.loadFromFile("sprites/settings.png");

	if (success) success = settingsTextField.loadMedia();

	return success;
}

void Settings::handleEvent(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		if (enteredOption != TOTAL_KEYS) {
			if (e.key.keysym.sym == SDLK_ESCAPE) enteredOption = TOTAL_KEYS;
			else {
				setKey(enteredOption, e.key.keysym.sym);
				enteredOption = TOTAL_KEYS;

				std::vector<int> codes = getKeys();
				settingsOptions[UP_KEY].setText("UP       " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[UP_KEY]))));
				settingsOptions[LEFT_KEY].setText("LEFT     " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[LEFT_KEY]))));
				settingsOptions[RIGHT_KEY].setText("RIGHT   " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[RIGHT_KEY]))));
				settingsOptions[SHOOT_KEY].setText("SHOOT   " + std::string(SDL_GetKeyName(static_cast<SDL_Keycode>(codes[SHOOT_KEY]))));
			}
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
					break;
				default:
					if (currentSelectedOption == NAME) settingsTextField.handleEvent(e);
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

	settingsTextField.setPosition(offsetX + SCREEN_WIDTH / 2 + OPTIONS_START_X, 130);
	settingsTextField.render();

	renderOptionHighlighter();

	settingsHeader[0].setPosition(offsetX + SCREEN_WIDTH / 2 + OPTIONS_START_X - 20, 90);
	settingsHeader[0].render();

	settingsHeader[1].setPosition(offsetX + SCREEN_WIDTH / 2 + OPTIONS_START_X - 20, OPTIONS_START_Y - 40);
	settingsHeader[1].render();

	for (int i = 1; i < TOTAL_KEYS; i++) {
		settingsOptions[i].setPosition(offsetX + SCREEN_WIDTH / 2 + OPTIONS_START_X, OPTIONS_START_Y + OPTIONS_HEIGHT * (i - 1));
		settingsOptions[i].render();
	}

	renderThinBorder();

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
	if (currentSelectedOption != NAME) settingsOptions[currentSelectedOption].renderHighlighter();
	else settingsTextField.renderBorder();
}

void Settings::renderThinBorder() {
	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);
	SDL_Rect optionThinBorderRect = {offsetX + SCREEN_WIDTH / 2 + OPTIONS_START_X - 30, OPTIONS_START_Y, SETTINGS_THIN_BORDER_WIDTH, OPTIONS_HEIGHT * (TOTAL_KEYS - 1) - 6};
	SDL_RenderFillRect(gRenderer, &optionThinBorderRect);
}

void Settings::close() {
	//Free global font
	TTF_CloseFont( optionsFont );
	optionsFont = NULL;
	for (int i = 0; i < TOTAL_KEYS; i++)
		settingsOptions[i].close();
}

void Settings::moveOffset() {
	offsetX += deltaX;
	if (offsetX <= 0) offsetX = 0;
	if (offsetX >= SCREEN_WIDTH / 2) offsetX = SCREEN_WIDTH / 2;
}