#include "MapSidebar.h"
#include "Block.h"
#include "Acid.h"
#include "Spike.h"
#include "Hero.h"

class Map {
	public:
		Map();
		Map(std::string _mode, int _level, std::vector<CollisionArea> _collisionAreas, Color _blockColor = Color(), Color _acidColor = Color());
		void setBackground(Color color = Color());
		bool loadMedia();
		int getWidth();
		int getHeight();
		void loadBlocks();
		void loadAcids();
		void loadSpikes();
		void loadHero();
		void handleEvent(SDL_Event &e);
		void move();
		void render(int offsetX);
		void reset();
		bool isGameOver;
		bool isLevelFinished;
	private:
		std::string mode;
		int level;
		int currentScore;
		int previousTime;
		LTexture *backgroundTexture; Color backgroundColor;
		int LEVEL_WIDTH, LEVEL_HEIGHT;
		std::vector<CollisionArea> collisionAreas;
		std::vector<Block> blocks;
		std::vector<Acid> acids;
		std::vector<Spike> spikes;
		Color blockColor, acidColor;
		Hero hero;
		MapSidebar mapSidebar;
};

Map::Map() {}
Map::Map(std::string _mode, int _level, std::vector<CollisionArea> _collisionAreas, Color _blockColor, Color _acidColor) {
	mode = _mode;
	level = _level;
	currentScore = 0;
	previousTime = -1;

	Color sidebarColor;
	if (_mode == "training") sidebarColor = {21, 24, 38, 0xFF};
	else if (_mode == "dodge") sidebarColor = {17, 44, 57, 0xFF};
	mapSidebar = MapSidebar(mode, level, sidebarColor);
	isGameOver = false;
	isLevelFinished = false;

	blockColor = _blockColor;
	acidColor = _acidColor;
	LEVEL_WIDTH = getWidth();
	LEVEL_HEIGHT = getHeight();
	loadBlocks();
	loadAcids();
	loadSpikes();
	loadHero();

	collisionAreas = _collisionAreas;
	for (CollisionArea &collisionArea : collisionAreas) {
		collisionArea.topLeftY += LEVEL_HEIGHT;
		collisionArea.bottomRightX += LEVEL_WIDTH;
		collisionArea.bottomRightY += LEVEL_HEIGHT;
	}

	// Include level boundaries as collision areas
	collisionAreas.push_back({RECTANGULAR_BOX, 0, 0, 0, LEVEL_HEIGHT}); // Left
	collisionAreas.push_back({RECTANGULAR_BOX, LEVEL_WIDTH, 0, LEVEL_WIDTH, LEVEL_HEIGHT}); // Right
	collisionAreas.push_back({RECTANGULAR_BOX, 0, 0, LEVEL_WIDTH, 0}); // Top
	collisionAreas.push_back({RECTANGULAR_BOX, 0, LEVEL_HEIGHT, LEVEL_WIDTH, LEVEL_HEIGHT}); // Bottom
}

void Map::setBackground(Color color) {
	backgroundTexture = new LTexture();
	bool success = backgroundTexture->loadFromFile(std::string("backgrounds/" + mode + (level >= 0 ? std::to_string(level + 1) : "") + ".png").c_str());
	if (!success) backgroundTexture = NULL;

	backgroundColor = color;
}

bool Map::loadMedia() {
	bool success = true;

	if (success) success = mapSidebar.loadMedia();
	if (success) success = Block::loadMedia();
	if (success) success = hero.loadMedia();

	return success;
}

int Map::getWidth() {
	std::ifstream mapFile(std::string("maps/" + mode + (level >= 0 ? std::to_string(level + 1) : "") + ".map").c_str());
	std::string mapStr;
	std::getline(mapFile, mapStr);
	int len = mapStr.length();
	if (len > 0 && mapStr[len - 1] == '\n') len--;
	mapFile.close();
	return len * 20;
}
int Map::getHeight() {
	std::ifstream mapFile(std::string("maps/" + mode + (level >= 0 ? std::to_string(level + 1) : "") + ".map").c_str());
	std::string mapStr;
	int lines = 0;
	while (mapFile >> mapStr) lines++;
	mapFile.close();
	return lines * 20;
}

void Map::loadBlocks() {
	std::ifstream mapFile(std::string("maps/" + mode + (level >= 0 ? std::to_string(level + 1) : "") + ".map").c_str());
	std::vector<std::string> rowString(LEVEL_HEIGHT);
	std::vector<std::vector<bool>> rowVis(LEVEL_HEIGHT, std::vector<bool>(LEVEL_WIDTH, false));
	int totalRows = LEVEL_HEIGHT / 20;
	int totalCols = LEVEL_WIDTH / 20;

	for (int atRow = 0; atRow < totalRows; atRow++) mapFile >> rowString[atRow];
	for (int atRow = 0; atRow < totalRows; atRow++) {
		for (int atCol = 0; atCol < totalCols; atCol++) if (!rowVis[atRow][atCol]) {
			rowVis[atRow][atCol] = true;
			if (rowString[atRow][atCol] == '#') {
				int lenX, lenY;
				for (lenX = 1; atCol + lenX < totalCols && rowString[atRow][atCol + lenX] == '-'; lenX++);
				for (lenY = 1; atRow + lenY < totalRows && rowString[atRow + lenY][atCol] == '|'; lenY++);

				int pathX, pathY;
				for (pathX = lenX; atCol + pathX < totalCols && rowString[atRow][atCol + pathX] == '_'; pathX++);
				for (pathY = lenY; atRow + pathY < totalRows && rowString[atRow + pathY][atCol] == '_'; pathY++);

				int dir = 0;
				if (pathX > lenX) dir = 1, pathY = lenY;
				else if (pathY > lenY) dir = 0, pathX = lenX;

				blocks.push_back(Block(atCol * 20, atRow * 20, lenX * 20, lenY * 20, pathX * 20, pathY * 20, blockColor, false, dir));
				for (int r = 0; r < pathY; r++) for (int c = 0; c < pathY; c++) {
					rowVis[atRow + r][atCol + c] = true;
				}
			}
			else if (rowString[atRow][atCol] == 'f') {
				blocks.push_back(Block(atCol * 20, atRow * 20, 20, 20, 20, 20, Color(206, 59, 136), true));
			}
		}
	}

	mapFile.close();
}

void Map::loadAcids() {
	std::ifstream mapFile(std::string("maps/" + mode + (level >= 0 ? std::to_string(level + 1) : "") + ".map").c_str());
	std::vector<std::string> rowString(LEVEL_HEIGHT);
	std::vector<std::vector<bool>> rowVis(LEVEL_HEIGHT, std::vector<bool>(LEVEL_WIDTH, false));
	int totalRows = LEVEL_HEIGHT / 20;
	int totalCols = LEVEL_WIDTH / 20;

	for (int atRow = 0; atRow < totalRows; atRow++) mapFile >> rowString[atRow];
	for (int atRow = 0; atRow < totalRows; atRow++) {
		for (int atCol = 0; atCol < totalCols; atCol++) if (!rowVis[atRow][atCol]) {
			rowVis[atRow][atCol] = true;
			if (rowString[atRow][atCol] == 'a') {
				int lenX, lenY;
				for (lenX = 1; atCol + lenX < totalCols && rowString[atRow][atCol + lenX] == '-'; lenX++);
				for (lenY = 1; atRow + lenY < totalRows && rowString[atRow + lenY][atCol] == '|'; lenY++);
				acids.push_back(Acid(atCol * 20, atRow * 20, lenX * 20, lenY * 20, acidColor));
				for (int r = 0; r < lenY; r++) for (int c = 0; c < lenX; c++) {
					rowVis[atRow + r][atCol + c] = true;
				}
			}
		}
	}

	mapFile.close();
}

void Map::loadSpikes() {
	std::ifstream mapFile(std::string("maps/" + mode + (level >= 0 ? std::to_string(level + 1) : "") + ".map").c_str());
	std::vector<std::string> rowString(LEVEL_HEIGHT);
	int totalRows = LEVEL_HEIGHT / 20;
	int totalCols = LEVEL_WIDTH / 20;

	for (int atRow = 0; atRow < totalRows; atRow++) mapFile >> rowString[atRow];
	for (int atRow = 0; atRow < totalRows; atRow++) {
		for (int atCol = 0; atCol < totalCols; atCol++) {
			int dir = -1;
			switch (rowString[atRow][atCol]) {
				case 'W':
					dir = 0;
					break;
				case 'D':
					dir = 1;
					break;
				case 'S':
					dir = 2;
					break;
				case 'A':
					dir = 3;
					break;
			}
			if (dir != -1) spikes.push_back(Spike(atCol * 20, atRow * 20, dir));
		}
	}

	mapFile.close();
}

void Map::loadHero() {
	std::ifstream mapFile(std::string("maps/" + mode + (level >= 0 ? std::to_string(level + 1) : "") + ".map").c_str());
	std::string mapStr;
	int atX = 0, atY = 0;
	while (mapFile >> mapStr) {
		atX = 0;
		for (char c : mapStr) {
			if (c == 'h') {
				hero.setInitialPosition(atX, atY);
				return;
			}
			atX += 20;
		}
		atY += 20;
	}
}

void Map::handleEvent(SDL_Event &e) {
	hero.handleEvent(e);
}

void Map::move() {
	for (Block &b : blocks) b.move(hero.isStanding(b), hero.getX(), hero.getY());
	hero.setObstacles(collisionAreas, blocks, acids, spikes);
	hero.move();

	if (hero.isDead) {
		mapSidebar.died();
		isGameOver = mapSidebar.isGameOver();
		SDL_Delay(1000);
		hero.reset();
	}
	else if (hero.isLevelFinished) {
		isLevelFinished = hero.isLevelFinished;
		mapSidebar.finished();
	}
}

void Map::render(int offsetX) {
	if (mode == "training") {
		if (offsetX == 0) { // If map is in full focuss
			mapSidebar.updateProgress(int(100.0 * (hero.getX() + 50) / LEVEL_WIDTH));
		}
	}
	else if (mode == "dodge") {
		if (offsetX == 0) { // If map is in full focuss
			mapSidebar.updateProgress(int(100.0 * (hero.getX() + 50) / LEVEL_WIDTH));

			if (previousTime != -1) mapSidebar.updateScore((SDL_GetTicks() - previousTime) / 1000.0);
			previousTime = SDL_GetTicks();
		}
		else if (previousTime != -1) previousTime = -1; // When map is losing focus
	}

	// Render map sidebar
	if (offsetX < MAP_SIDEBAR_WIDTH) {
		mapSidebar.render();
		offsetX = MAP_SIDEBAR_WIDTH;
	}

	int clipOffset = std::max(0, std::min(LEVEL_WIDTH - (SCREEN_WIDTH - MAP_SIDEBAR_WIDTH) / 2, hero.getX()) - (SCREEN_WIDTH - MAP_SIDEBAR_WIDTH) / 2);

	// Render background
	if (backgroundTexture != NULL) {
		int backgroundWidth = backgroundTexture->getWidth();
		int backgroundHeight = backgroundTexture->getHeight();
		SDL_Rect backgroundClipRect = {clipOffset % backgroundWidth, 0, backgroundWidth - (clipOffset % backgroundWidth), backgroundHeight};
		backgroundTexture->render(offsetX, SCREEN_HEIGHT - backgroundHeight, &backgroundClipRect);
		backgroundTexture->render(offsetX + backgroundClipRect.w, SCREEN_HEIGHT - backgroundHeight);
		backgroundTexture->render(offsetX + backgroundClipRect.w + backgroundWidth, SCREEN_HEIGHT - backgroundHeight);
	}
	else {
		SDL_SetRenderDrawColor(gRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		SDL_Rect backgroundRect = {offsetX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_RenderFillRect(gRenderer, &backgroundRect);
	}

	// Render blocks
	for (Block &block : blocks) block.render(offsetX, clipOffset);

	// Render acids
	for (Acid &acid : acids) acid.render(offsetX, clipOffset);

	// Render spikes
	for (Spike &spike : spikes) spike.render(offsetX, clipOffset);

	// Render hero
	if (hero.isStanding()) hero.renderShadow(offsetX, clipOffset);
	hero.render(offsetX, clipOffset);
}

void Map::reset() {
	mapSidebar.reset();
	isGameOver = false;
	isLevelFinished = false;
}