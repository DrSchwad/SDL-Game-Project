TTF_Font *optionsFont = NULL;
SDL_Color settingsTextColor = { 0xFF, 0xFF, 0xFF };
const int SETTINGS_THIN_BORDER_WIDTH = 2;
const int SETTINGS_BORDER_WIDTH = 10;
const int SETTINGS_BORDER_HEIGHT = 44;
const int SETTINGS_BACKGROUND_WIDTH = 250;

class SettingsOption {
	public:
		SettingsOption();
		bool setText(std::string str);
		void setPosition(int x, int y);
		void render();
		void renderHighlighter();
		void resetHighlighter();
		void renderBorder();
		void close();
	private:
		LTexture optionTextTexture;
		int posX, posY;
		int highligherCurrentWidth, highligherDeltaWidth;
};

SettingsOption::SettingsOption() {
	highligherCurrentWidth = 0;
	highligherDeltaWidth = 15;
}

bool SettingsOption::setText(std::string str) {
	return optionTextTexture.loadFromRenderedText(str, settingsTextColor, optionsFont);
}

void SettingsOption::setPosition(int x, int y) {
	posX = x;
	posY = y;
}

void SettingsOption::render() {
	optionTextTexture.render(posX, posY);
}

void SettingsOption::renderHighlighter() {
	SDL_SetRenderDrawColor(gRenderer, 38, 44, 58, 0xFF);
	highligherCurrentWidth += highligherDeltaWidth;
	if (highligherCurrentWidth > SETTINGS_BACKGROUND_WIDTH) highligherCurrentWidth = SETTINGS_BACKGROUND_WIDTH;
	SDL_Rect optionBackgroundRect = {posX - 30 + SETTINGS_THIN_BORDER_WIDTH, posY, highligherCurrentWidth, SETTINGS_BORDER_HEIGHT};
	SDL_RenderFillRect(gRenderer, &optionBackgroundRect);
}

void SettingsOption::resetHighlighter() {
	highligherCurrentWidth = 0;
}

void SettingsOption::renderBorder() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect optionBorderRect = {posX - 30, posY, SETTINGS_BORDER_WIDTH, SETTINGS_BORDER_HEIGHT};
	SDL_RenderFillRect(gRenderer, &optionBorderRect);
}

void SettingsOption::close() {
	optionTextTexture.free();
}