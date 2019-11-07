TTF_Font *settingsheaderFont = NULL;
SDL_Color settingsHeaderTextColor = { 0xAA, 0xAA, 0xAA };

class SettingsHeader {
	public:
		SettingsHeader();
		bool setText(std::string str);
		void setPosition(int x, int y);
		void render();
		void close();
	private:
		LTexture optionHeaderTextTexture;
		int posX, posY;
};

SettingsHeader::SettingsHeader() {}

bool SettingsHeader::setText(std::string str) {
	return optionHeaderTextTexture.loadFromRenderedText(str, settingsHeaderTextColor, settingsheaderFont);
}

void SettingsHeader::setPosition(int x, int y) {
	posX = x;
	posY = y;
}

void SettingsHeader::render() {
	optionHeaderTextTexture.render(posX, posY);
}

void SettingsHeader::close() {
	optionHeaderTextTexture.free();
}