const int SIDEBAR_BACKTOMENU_FONT_SIZE = 22;
const int SIDEBAR_MODEANDLEVEL_FONT_SIZE = 30;
const int SIDEBAR_PROGRESSCOUNTER_FONT_SIZE = 18;
const int SIDEBAR_SCORETEXT_FONT_SIZE = 18;
const int SIDEBAR_SCORECOUNTER_FONT_SIZE = 40;
const int MAP_SIDEBAR_WIDTH = 300;
SDL_Color menuSidebarColor = { 0xFF, 0xFF, 0xFF };

class MapSidebar {
	public:
		MapSidebar();
		MapSidebar(std::string mode, int level, Color backgroundColor = Color());
		bool loadMedia();
		void updateProgress(int _progress);
		void updateScore(double _score);
		void render();
		void died();
		void finished();
		bool isGameOver();
		void reset();
	private:
		std::string mode;
		int level;
		int lives;
		int progress;
		double score;
		double highestScore;
		Color backgroundColor;

		// Text textures
		TTF_Font *backToMenuFont;
		TTF_Font *modeAndLevelFont;
		TTF_Font *progressCounterFont;
		TTF_Font *scoreTextFont;
		TTF_Font *scoreCounterFont;
		LTexture backToMenuTextTexture;
		LTexture modeAndLevelTextTexture;
		LTexture modeAndLevelCounterTexture;
		LTexture progressCounterTexture;
		LTexture heartTexture;
		LTexture scoreTextTexture;
		LTexture scoreCounterTexture;
		LTexture hScoreTextTexture;
		LTexture hScoreCounterTexture;
};

MapSidebar::MapSidebar() {
}

MapSidebar::MapSidebar(std::string mode, int level, Color backgroundColor) : mode(mode), level(level), backgroundColor(backgroundColor) {
	if (mode == "training") lives = 1;
	else if (mode == "dodge") lives = 3;

	progress = 0;
	score = 0;

	if (mode == "training") {
		score = -1;
		highestScore = -1;
	}
	else if (mode == "dodge") {
		std::ifstream leaderboardFile(std::string("data/dodge leaderboard.txt"));
		for (int i = 0; i < level; i++) leaderboardFile >> highestScore;
		if (!(leaderboardFile >> highestScore)) highestScore = -1;
		leaderboardFile.close();
	}
}

bool MapSidebar::loadMedia() {
	bool success = true;

	if (success) {
		backToMenuFont = TTF_OpenFont( "fonts/map sidebar font.ttf", SIDEBAR_BACKTOMENU_FONT_SIZE );
		if (backToMenuFont == NULL) {
			printf("Failed to load map sidebar font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}
	if (success) {
		modeAndLevelFont = TTF_OpenFont( "fonts/map sidebar font.ttf", SIDEBAR_MODEANDLEVEL_FONT_SIZE );
		if (modeAndLevelFont == NULL) {
			printf("Failed to load map sidebar font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}
	if (success) {
		progressCounterFont = TTF_OpenFont( "fonts/map sidebar font.ttf", SIDEBAR_PROGRESSCOUNTER_FONT_SIZE );
		if (progressCounterFont == NULL) {
			printf("Failed to load map sidebar font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}
	if (success) {
		scoreTextFont = TTF_OpenFont( "fonts/map sidebar font.ttf", SIDEBAR_SCORETEXT_FONT_SIZE );
		if (scoreTextFont == NULL) {
			printf("Failed to load map sidebar font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}
	if (success) {
		scoreCounterFont = TTF_OpenFont( "fonts/map sidebar font.ttf", SIDEBAR_SCORECOUNTER_FONT_SIZE );
		if (scoreCounterFont == NULL) {
			printf("Failed to load map sidebar font! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}

	if (success) {
		backToMenuTextTexture.loadFromRenderedText("BACK TO MENU (ESC)", menuSidebarColor, backToMenuFont);
		std::string modeAndLevelTextString;
		if (mode == "training") modeAndLevelTextString = "TRAINING MODE";
		else if (mode == "dodge") modeAndLevelTextString = "DODGE MODE";
		modeAndLevelTextTexture.loadFromRenderedText(modeAndLevelTextString, menuSidebarColor, modeAndLevelFont);
		scoreTextTexture.loadFromRenderedText("SCORE:", menuSidebarColor, scoreTextFont);
		hScoreTextTexture.loadFromRenderedText("HIGHEST SCORE:", menuSidebarColor, scoreTextFont);
	}

	if (success) success = heartTexture.loadFromFile("sprites/heart.png");

	return success;
}

void MapSidebar::updateProgress(int _progress) {
	progress = _progress;
}

void MapSidebar::updateScore(double _score) {
	score += _score;
}

void MapSidebar::died() {
	lives--;
}

void MapSidebar::finished() {
	if (mode == "dodge") {
		std::ifstream iLeaderboardFile(std::string("data/dodge leaderboard.txt"));
		
		std::vector<double> scores(TOTAL_DODGE_LEVELS);
		for (int i = 0; i < TOTAL_DODGE_LEVELS; i++) {
			if (!(iLeaderboardFile >> scores[i])) scores[i] = -1;
		}

		if (scores[level] >= 0) scores[level] = std::min(scores[level], score);
		else scores[level] = score;
		highestScore = scores[level];

		iLeaderboardFile.close();

		std::ofstream oLeaderboardFile(std::string("data/dodge leaderboard.txt"));
		std::string scoreStr = "";
		for (int i = 0; i < TOTAL_DODGE_LEVELS; i++) scoreStr += std::to_string(scores[i]) + " ";
		oLeaderboardFile << scoreStr;
		oLeaderboardFile.close();
	}
}

bool MapSidebar::isGameOver() {
	return lives == 0;
}

void MapSidebar::reset() {
	if (mode == "training") lives = 1;
	else if (mode == "dodge") lives = 3;

	progress = 0;
	score = 0;
}

void MapSidebar::render() {
	// Render background
	SDL_SetRenderDrawColor(gRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_Rect mapSidebarRect = {0, 0, MAP_SIDEBAR_WIDTH, SCREEN_HEIGHT};
	SDL_RenderFillRect(gRenderer, &mapSidebarRect);

	// Render the texts and hearts
	backToMenuTextTexture.render(25, 30);
	modeAndLevelTextTexture.render(20, 130);
	if (mode == "dodge") {
		modeAndLevelCounterTexture.loadFromRenderedText("(LVL " + std::to_string(level + 1) + ")", menuSidebarColor, modeAndLevelFont);
		modeAndLevelCounterTexture.render(180, 130);
	}

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect progressRect = {20, 170, MAP_SIDEBAR_WIDTH - 40, 10};
	SDL_RenderFillRect(gRenderer, &progressRect);
	int currentPos = 20 + int((MAP_SIDEBAR_WIDTH - 40) / 100.0 * progress);
	SDL_Rect indicatorRect = {currentPos, 170, 10, 20};
	SDL_RenderFillRect(gRenderer, &indicatorRect);
	progressCounterTexture.loadFromRenderedText(std::to_string(progress) + "%", menuSidebarColor, progressCounterFont);
	progressCounterTexture.render(currentPos, 190);

	int heartTextureOffset = (MAP_SIDEBAR_WIDTH - (lives * 40 + (40 - heartTexture.getWidth()))) / 2;
	for (int i = 0; i < lives; i++) heartTexture.render(heartTextureOffset + 20 + i * 40, 265);

	std::string scoreText, hScoreText;
	if (mode == "training") {
		scoreText = "-";
		hScoreText = "-";
	}
	else if (mode == "dodge") {
		scoreText = to_string_with_precision<double>(score, 2) + "s";

		hScoreText = "-";
		if (highestScore >= 0) hScoreText = to_string_with_precision<double>(highestScore, 2) + "s";
	}
	scoreCounterTexture.loadFromRenderedText(scoreText, menuSidebarColor, scoreCounterFont);
	hScoreCounterTexture.loadFromRenderedText(hScoreText, menuSidebarColor, scoreCounterFont);
	scoreTextTexture.render(20, 350); scoreCounterTexture.render(70, 370);
	hScoreTextTexture.render(20, 460); hScoreCounterTexture.render(70, 480);
}