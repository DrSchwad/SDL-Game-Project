const int LEADERBOARD_TITLE_FONT_SIZE = 40;
const int LEADERBOARD_LEVELTEXT_FONT_SIZE = 20;
const int LEADERBOARD_SCORECOUNTER_FONT_SIZE = 40;

SDL_Color leaderboardColor = { 0xFF, 0xFF, 0xFF };

class Leaderboard {
	public:
		Leaderboard();
		bool loadMedia();
		void render();
	private:
		// Text textures
		TTF_Font *titleFont;
		TTF_Font *levelTextFont;
		TTF_Font *scoreCounterFont;
		LTexture titleTexture;
		LTexture levelTextTexture;
		LTexture scoreCounterTexture;
};

Leaderboard::Leaderboard() {}

bool Leaderboard::loadMedia() {
	bool success = true;

	if (success) {
		titleFont = TTF_OpenFont( "fonts/leaderboard font.ttf", LEADERBOARD_TITLE_FONT_SIZE );
		if (titleFont == NULL) {
			printf("Failed to load leaderboard font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}
	if (success) {
		levelTextFont = TTF_OpenFont( "fonts/leaderboard font.ttf", LEADERBOARD_LEVELTEXT_FONT_SIZE );
		if (levelTextFont == NULL) {
			printf("Failed to load leaderboard font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}
	if (success) {
		scoreCounterFont = TTF_OpenFont( "fonts/leaderboard font.ttf", LEADERBOARD_SCORECOUNTER_FONT_SIZE );
		if (scoreCounterFont == NULL) {
			printf("Failed to load leaderboard font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}

	return success;
}

void Leaderboard::render() {
	// Render background
	SDL_SetRenderDrawColor(gRenderer, 21, 24, 38, 0xEE);
	SDL_Rect quitBackgroundRect = {SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
	SDL_RenderFillRect(gRenderer, &quitBackgroundRect);

	// Render texts
	titleTexture.loadFromRenderedText("DODGE MODE", leaderboardColor, titleFont);
	titleTexture.render(SCREEN_WIDTH / 2 + 100, 100);

	std::ifstream leaderboardFile(std::string("data/leaderboard.txt"));

	for (int i = 0; i < TOTAL_DODGE_LEVELS; i++) {
		levelTextTexture.loadFromRenderedText("LEVEL " + std::to_string(i + 1), leaderboardColor, levelTextFont);
		levelTextTexture.render(SCREEN_WIDTH / 2 + 200, 170 + i * 70);

		double score;
		if (!(leaderboardFile >> score)) score = -1;
		if (score >= 0) scoreCounterTexture.loadFromRenderedText(to_string_with_precision<double>(score, 2) + "s", leaderboardColor, scoreCounterFont);
		else scoreCounterTexture.loadFromRenderedText("-", leaderboardColor, scoreCounterFont);
		scoreCounterTexture.render(SCREEN_WIDTH / 2 + 300, 170 + i * 70);
	}

	leaderboardFile.close();
}