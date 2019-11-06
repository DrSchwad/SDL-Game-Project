class Block {
	public:
		Block(int startX, int startY, int lenX, int lenY, int pathX, int pathY, Color color, bool finishingBlock = false, int dir = 0);
		static bool loadMedia();
		void move(bool moveHero, int &heroX, int &heroY);
		void render(int offsetX, int clipOffset);
		CollisionArea collisionArea;
		bool finishingBlock;
	private:
		int startX, startY;
		int pathX, pathY;
		int posX, posY;
		int lenX, lenY;
		int dir;
		int dx, dy;
		Color color;
};

Block::Block(int startX, int startY, int lenX, int lenY, int pathX, int pathY, Color color, bool finishingBlock, int dir) : startX(startX), startY(startY), lenX(lenX), lenY(lenY), pathX(pathX), pathY(pathY), color(color), finishingBlock(finishingBlock), dir(dir) {
	posX = startX;
	posY = startY;

	collisionArea = CollisionArea(RECTANGULAR_BOX, posX, posY, posX + lenX, posY + lenY);
}

bool Block::loadMedia() {
	bool success = true;
	return success;
}

void Block::move(bool moveHero, int &heroX, int &heroY) {
	switch (dir) {
		case 0:
			dx = 0, dy = -5;
			break;
		case 1:
			dx = 5, dy = 0;
			break;
		case 2:
			dx = 0, dy = 5;
			break;
		case 3:
			dx = -5, dy = 0;
			break;
	}

	int initX = posX, initY = posY;
	posX += dx, posY += dy;
	bool changeDir = false;
	if (posX < startX || posX + lenX > startX + pathX) posX -= dx, changeDir = true;
	if (posY < startY || posY + lenY > startY + pathY) posY -= dy, changeDir = true;
	if (changeDir) dir = (dir + 2) % 4;

	collisionArea = CollisionArea(RECTANGULAR_BOX, posX, posY, posX + lenX, posY + lenY);

	if (moveHero) heroX += (posX - initX), heroY += (posY - initY);
}

void Block::render(int offsetX, int clipOffset) {
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);

	int renderX = std::max(offsetX + (posX - clipOffset), offsetX);
	int rendLen = std::max(0, std::min(lenX, (posX - clipOffset) + lenX));
	if (rendLen == 0) return;

	SDL_Rect blockRect = {renderX, posY, rendLen, lenY};
	SDL_RenderFillRect(gRenderer, &blockRect);
}