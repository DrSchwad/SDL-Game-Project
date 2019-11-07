const int SETTINGS_TEXTFIELD_FONT_SIZE = 28;
const int SETTINGS_TEXTFIELD_HEIGHT = 28;
const int SETTINGS_TEXTFIELD_BORDER_WIDTH = 28;

class SettingsTextField {
	public:
		SettingsTextField();
		bool loadMedia();
		void setBackgroundColor(Color c);
		void setTextColor(Color c);
		void setHintTextColor(Color c);
		void setHintText(std::string str);
		void setPosition(int x, int y);
		void handleEvent(SDL_Event &e);
		void render();
		void renderBorder();
		void close();

	private:
		TTF_Font *font = NULL;
		Color backgroundColor;
		Color textColor;
		Color hintTextColor;
		std::string hintText;
		int posX, posY;
};

SettingsTextField::SettingsTextField() {}

bool SettingsTextField::loadMedia() {
	bool success = true;

	if (success) {
		font = TTF_OpenFont( "fonts/settings font.ttf", SETTINGS_TEXTFIELD_FONT_SIZE );
		if (font == NULL) {
			printf("Failed to load settings text field font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}

	return true;
}

void SettingsTextField::setBackgroundColor(Color c) {backgroundColor = c;}
void SettingsTextField::setTextColor(Color c) {textColor = c;}
void SettingsTextField::setHintTextColor(Color c) {hintTextColor = c;}
void SettingsTextField::setHintText(std::string str) {hintText = str;}
void SettingsTextField::setPosition(int x, int y) {posX = x, posY = y;}

void SettingsTextField::handleEvent(SDL_Event &e) {

}

void SettingsTextField::render() {
	SDL_SetRenderDrawColor(gRenderer, 38, 44, 58, 0xFF);
	SDL_Rect textFieldBackgroundRect = {posX - 30 + SETTINGS_THIN_BORDER_WIDTH, posY, SETTINGS_TEXTFIELD_BORDER_WIDTH, SETTINGS_TEXTFIELD_HEIGHT};
	SDL_RenderFillRect(gRenderer, &textFieldBackgroundRect);
}

void SettingsTextField::renderBorder() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect textFieldBorderRect = {posX - 30, posY, SETTINGS_TEXTFIELD_BORDER_WIDTH, SETTINGS_TEXTFIELD_HEIGHT};
	SDL_RenderFillRect(gRenderer, &textFieldBorderRect);
}

void SettingsTextField::close() {

}