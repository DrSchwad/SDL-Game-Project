const int POPUP_HEADER_FONT_SIZE = 60;
const int POPUP_TEXT_FONT_SIZE = 24;

class Popup {
	public:
		Popup();
		Popup(std::string header, std::string text);
		bool loadMedia();
		void render();
	private:
		TTF_Font *headerFont = NULL, *textFont = NULL;
		LTexture headerTexture, textTexture;
		std::string header, text;
};

Popup::Popup() {}
Popup::Popup(std::string header, std::string text) : header(header), text(text) {}

bool Popup::loadMedia() {
	bool success = true;

	headerFont = TTF_OpenFont( "fonts/popup header.ttf", POPUP_HEADER_FONT_SIZE );
	if (headerFont == NULL) {
		printf("Failed to load popup header font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	textFont = TTF_OpenFont( "fonts/popup text.ttf", POPUP_TEXT_FONT_SIZE );
	if (textFont == NULL) {
		printf("Failed to load popup text font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	headerTexture.loadFromRenderedText(header, {0xFF, 0xFF, 0xFF}, headerFont);
	textTexture.loadFromRenderedText(text, {0xFF, 0xFF, 0xFF}, textFont);

	return success;
}

void Popup::render() {
	headerTexture.render(300 + (SCREEN_WIDTH - 300 - headerTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - headerTexture.getHeight() - 5);
	textTexture.render(300 + (SCREEN_WIDTH - 300 - textTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 + 5);
}