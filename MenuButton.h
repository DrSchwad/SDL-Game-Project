TTF_Font *menuFont = NULL;
SDL_Color menuTextColor = { 0xFF, 0xFF, 0xFF };
const int MENU_BORDER_WIDTH = 10;
const int MENU_BORDER_HEIGHT = 44;
const int MENU_BACKGROUND_WIDTH = 210;

class MenuButton {
	public:
		MenuButton();
		bool setText(std::string str);
		void setPosition(int x, int y);
		void render();
		void renderHighlighter();
		void resetHighlighter();
		void close();
		bool backToMenu();
	private:
		LTexture buttonTextTexture;
		int posX, posY;
		int highligherCurrentWidth, highligherDeltaWidth;
};

MenuButton::MenuButton() {
	highligherCurrentWidth = 0;
	highligherDeltaWidth = 15;
}

bool MenuButton::setText(std::string str) {
	return buttonTextTexture.loadFromRenderedText(str, menuTextColor, menuFont);
}

void MenuButton::setPosition(int x, int y) {
	posX = x;
	posY = y;
}

void MenuButton::render() {
	buttonTextTexture.render(posX, posY);
}

void MenuButton::renderHighlighter() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect buttonBorderRect = {posX - 30, posY, MENU_BORDER_WIDTH, MENU_BORDER_HEIGHT};
	SDL_RenderFillRect(gRenderer, &buttonBorderRect);

	SDL_SetRenderDrawColor(gRenderer, 38, 44, 58, 0xFF);
	highligherCurrentWidth += highligherDeltaWidth;
	if (highligherCurrentWidth > MENU_BACKGROUND_WIDTH) highligherCurrentWidth = MENU_BACKGROUND_WIDTH;
	SDL_Rect buttonBackgroundRect = {posX - 30 + MENU_BORDER_WIDTH, posY, highligherCurrentWidth, MENU_BORDER_HEIGHT};
	SDL_RenderFillRect(gRenderer, &buttonBackgroundRect);
}

void MenuButton::resetHighlighter() {
	highligherCurrentWidth = 0;
}

void MenuButton::close() {
	buttonTextTexture.free();
}