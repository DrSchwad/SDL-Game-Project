const int SETTINGS_TEXTFIELD_FONT_SIZE = 28;
const int SETTINGS_TEXTFIELD_HEIGHT = 40;
const int SETTINGS_TEXTFIELD_BORDER_WIDTH = 10;
const int SETTINGS_TEXTFIELD_BACKGROUND_WIDTH = 250;

class SettingsTextField {
	public:
		SettingsTextField();
		bool loadMedia();
		void setBackgroundColor(Color c);
		void setTextColor(SDL_Color c);
		void setHintTextColor(SDL_Color c);
		void setHintText(std::string str);
		void setPosition(int x, int y);
		std::string getName();
		void setName(std::string);
		void handleEvent(SDL_Event &e);
		void render();
		void renderBorder();
		void close();

	private:
		TTF_Font *font = NULL;
		LTexture nameTexture;
		Color backgroundColor;
		SDL_Color textColor;
		SDL_Color hintTextColor;
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
void SettingsTextField::setTextColor(SDL_Color c) {textColor = c;}
void SettingsTextField::setHintTextColor(SDL_Color c) {hintTextColor = c;}
void SettingsTextField::setHintText(std::string str) {hintText = str;}
void SettingsTextField::setPosition(int x, int y) {posX = x, posY = y;}

std::string SettingsTextField::getName() {
	std::string name = "";
	std::ifstream nameFile(std::string("data/name.txt"));
	nameFile >> name;
	nameFile.close();
	return name;
}
void SettingsTextField::setName(std::string new_name) {
	std::ofstream nameFile(std::string("data/name.txt"));
	nameFile << new_name;
	nameFile.close();
}

void SettingsTextField::handleEvent(SDL_Event &e) {
	std::string current_name = getName();

	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_BACKSPACE) {
			if (current_name.length()) current_name.pop_back();
		}
		else if (current_name.length() <= 11 && 'a' <= e.key.keysym.sym && e.key.keysym.sym <= 'z') {
			current_name += e.key.keysym.sym + 'A' - 'a';
		}
	}

	setName(current_name);
}

void SettingsTextField::render() {
	SDL_SetRenderDrawColor(gRenderer, 38, 44, 58, 0xFF);
	SDL_Rect textFieldBackgroundRect = {posX - 30 + SETTINGS_THIN_BORDER_WIDTH, posY, SETTINGS_TEXTFIELD_BACKGROUND_WIDTH, SETTINGS_TEXTFIELD_HEIGHT};
	SDL_RenderFillRect(gRenderer, &textFieldBackgroundRect);

	if (getName() != "") nameTexture.loadFromRenderedText(getName(), textColor, font);
	else nameTexture.loadFromRenderedText("Input username", hintTextColor, font);
	nameTexture.render(posX - 30 + SETTINGS_THIN_BORDER_WIDTH + 15, posY);
}

void SettingsTextField::renderBorder() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect textFieldBorderRect = {posX - 30, posY, SETTINGS_TEXTFIELD_BORDER_WIDTH, SETTINGS_TEXTFIELD_HEIGHT};
	SDL_RenderFillRect(gRenderer, &textFieldBorderRect);
}

void SettingsTextField::close() {

}