#include "MenuButton.h"

TTF_Font *gameTitleFont = NULL;
TTF_Font *levelTextFont = NULL;
LTexture gameTitleTextTexture;
LTexture menuLeftArrowTexture;
LTexture menuRightArrowTexture;

const int LEVEL_TEXT_FONT_SIZE = 80;

const int GAME_TITLE_FONT_SIZE = 100;
const int GAME_TITLE_POS_X = 140, GAME_TITLE_POS_Y = 50;

const int MENU_FONT_SIZE = 28;
const int BUTTON_START_X = 220, BUTTON_START_Y = 220;
const int BUTTON_HEIGHT = 50;

enum MenuOption {
	TRAINING_OPTION,
	DODGE_OPTION,
	SHOOT_OPTION,
	LEADERBOARD_OPTION,
	SETTINGS_OPTION,
	QUIT_OPTION,
	TOTAL_OPTIONS
};

class Menu {
	public:
		Menu();
		bool loadMedia();
		void handleEvent(SDL_Event &e);
		MenuOption getEnteredOption();
		void resetEnteredOption();
		MenuOption getCurrentSelectedOption();
		void render();
		void renderLevelIndicator(int level);
		void close();
		void moveOffset();
		int offsetX, deltaX;
		int levelDelta;
	private:
		LTexture levelText;
		MenuButton menuButtons[TOTAL_OPTIONS];
		MenuOption currentSelectedOption;
		MenuOption enteredOption;
		void renderMenuBackground();
		void renderGameTitle();
		void renderButtonHighlighter();
		void resetButtonHighlighter();
};

Menu::Menu() {
	offsetX = 0;
	deltaX = 0;
	levelDelta = 0;
	currentSelectedOption = TRAINING_OPTION;
	enteredOption = TOTAL_OPTIONS;
}

bool Menu::loadMedia() {
	bool success = true;

	// Open the font
	menuFont = TTF_OpenFont( "fonts/menu font.ttf", MENU_FONT_SIZE );
	if (menuFont == NULL) {
		printf("Failed to load menu font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	if (success) {
		success = menuButtons[TRAINING_OPTION].setText("TRAINING");
		success = menuButtons[DODGE_OPTION].setText("DODGE");
		success = menuButtons[SHOOT_OPTION].setText("SHOOT");
		success = menuButtons[LEADERBOARD_OPTION].setText("LEADERBOARD");
		success = menuButtons[SETTINGS_OPTION].setText("SETTINGS");
		success = menuButtons[QUIT_OPTION].setText("QUIT");
	}

	if (success) {
		gameTitleFont = TTF_OpenFont( "fonts/game title font.ttf", GAME_TITLE_FONT_SIZE );
		if (gameTitleFont == NULL) {
			printf("Failed to load game title font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}

	if (success) {
		if (!gameTitleTextTexture.loadFromRenderedText("MR. GUN", menuTextColor, gameTitleFont)) success = false;
	}

	if (success) success = menuLeftArrowTexture.loadFromFile("sprites/left arrow.png");
	if (success) success = menuRightArrowTexture.loadFromFile("sprites/right arrow.png");
	if (success) {
		levelTextFont = TTF_OpenFont( "fonts/level text font.ttf", LEVEL_TEXT_FONT_SIZE );
		if (levelTextFont == NULL) {
			printf("Failed to load level text font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}

	return success;
}

void Menu::handleEvent(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		switch( e.key.keysym.sym ) {
			case SDLK_UP:
				resetButtonHighlighter();
				currentSelectedOption = static_cast<MenuOption>((currentSelectedOption - 1 + TOTAL_OPTIONS) % TOTAL_OPTIONS);
				break;
			case SDLK_DOWN:
				resetButtonHighlighter();
				currentSelectedOption = static_cast<MenuOption>((currentSelectedOption + 1) % TOTAL_OPTIONS);
				break;
			case SDLK_LEFT:
				levelDelta = -1;
				break;
			case SDLK_RIGHT:
				levelDelta = 1;
				break;
			case SDLK_RETURN:
				enteredOption = currentSelectedOption;
		}
	}
}

MenuOption Menu::getEnteredOption() {
	return enteredOption;
}
void Menu::resetEnteredOption() {
	enteredOption = TOTAL_OPTIONS;
}

MenuOption Menu::getCurrentSelectedOption() {
	return currentSelectedOption;
}

void Menu::render() {
	renderMenuBackground();
	renderGameTitle();

	renderButtonHighlighter();

	for (int i = 0; i < TOTAL_OPTIONS; i++) {
		menuButtons[i].setPosition(offsetX + BUTTON_START_X, BUTTON_START_Y + BUTTON_HEIGHT * i);
		menuButtons[i].render();
	}
}

void Menu::renderMenuBackground() {
	SDL_SetRenderDrawColor(gRenderer, 21, 24, 38, 0xFF);
	SDL_Rect menuBackgroundRect = {offsetX, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
	SDL_RenderFillRect(gRenderer, &menuBackgroundRect);
}

void Menu::renderLevelIndicator(int level) {
	levelText.loadFromRenderedText("LEVEL " + std::to_string(level + 1), menuTextColor, levelTextFont);
	levelText.render(SCREEN_WIDTH / 2 + (SCREEN_WIDTH / 2 - levelText.getWidth()) / 2, 20);
	menuLeftArrowTexture.render(SCREEN_WIDTH / 2 + 50, (SCREEN_HEIGHT - menuLeftArrowTexture.getHeight()) / 2);
	menuRightArrowTexture.render(SCREEN_WIDTH - 50 - menuRightArrowTexture.getWidth(), (SCREEN_HEIGHT - menuRightArrowTexture.getHeight()) / 2);
}

void Menu::renderGameTitle() {
	gameTitleTextTexture.render(offsetX + GAME_TITLE_POS_X, GAME_TITLE_POS_Y);
}

void Menu::resetButtonHighlighter() {
	menuButtons[currentSelectedOption].resetHighlighter();
}

void Menu::renderButtonHighlighter() {
	menuButtons[currentSelectedOption].renderHighlighter();
}

void Menu::close() {
	//Free global font
	TTF_CloseFont( menuFont );
	menuFont = NULL;
	for (int i = 0; i < TOTAL_OPTIONS; i++)
		menuButtons[i].close();
}

void Menu::moveOffset() {
	offsetX += deltaX;
	if (offsetX <= -SCREEN_WIDTH / 2) offsetX = -SCREEN_WIDTH / 2;
	if (offsetX >= 0) offsetX = 0;
}