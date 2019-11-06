const int SPIKE_FRAMES = 4;
LTexture *SpikeSpriteSheetTexture;
SDL_Rect SpikeSpriteClips[SPIKE_FRAMES];

class Spike {
	public:
		Spike(int posX, int posY, int dir);
		bool loadMedia();
		void render(int offsetX, int clipOffset);
		CollisionArea collisionArea;
	private:
		int dir;
		int posX, posY;
};

Spike::Spike(int posX, int posY, int dir) : posX(posX), posY(posY), dir(dir) {
	collisionArea = CollisionArea(RECTANGULAR_BOX, posX, posY, posX + 20, posY + 20);
	if (SpikeSpriteSheetTexture == NULL) loadMedia();
}

bool Spike::loadMedia() {
	bool success = true;

	SpikeSpriteSheetTexture = new LTexture();
	if (success) {
		if (!SpikeSpriteSheetTexture->loadFromFile( "sprites/Spike.png") ) {
			printf( "Failed to load spike texture!\n" );
			success = false;
		}
		else {
			// Set sprite clips
			for (int i = 0; i < SPIKE_FRAMES; i++) {
				SpikeSpriteClips[i].x = 20 * i;
				SpikeSpriteClips[i].y = 0;
				SpikeSpriteClips[i].w = 20;
				SpikeSpriteClips[i].h = 20;
			}
		}
	}

	return success;
}

void Spike::render(int offsetX, int clipOffset) {
	int renderX = offsetX + (posX - clipOffset);

	SDL_Rect* currentClip;
	if (renderX >= offsetX) currentClip = currentClip = &SpikeSpriteClips[dir];
	else if (renderX + 20 > offsetX) {
		int intersection = (renderX + 20) - offsetX;
		currentClip = new SDL_Rect();

		currentClip->x = 20 - intersection;
		currentClip->y = 0;
		currentClip->w = intersection;
		currentClip->h = 20;
	}
	else return;

	SpikeSpriteSheetTexture->render(std::max(renderX, offsetX), posY, currentClip);
}