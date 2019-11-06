// Stance animation
const int ACID_ANIMATION_FRAMES = 4;
const int ACID_ANIMATION_UNIT = 4;
LTexture *AcidSpriteSheetTexture;
SDL_Rect AcidSpriteClips[ACID_ANIMATION_FRAMES];

class Acid {
	public:
		Acid(int posX, int posY, int lenX, int lenY, Color color);
		bool loadMedia();
		void render(int offsetX, int clipOffset);
		CollisionArea collisionArea;
	private:
		int posX, posY;
		int lenX, lenY;
		Color color;
		int animationState;
};

Acid::Acid(int posX, int posY, int lenX, int lenY, Color color) : posX(posX), posY(posY), lenX(lenX), lenY(lenY), color(color) {
	collisionArea = CollisionArea(RECTANGULAR_BOX, posX, posY + 10, posX + lenX, posY + lenY - 10);
	animationState = 0;
	if (AcidSpriteSheetTexture == NULL) loadMedia();
}

bool Acid::loadMedia() {
	bool success = true;

	AcidSpriteSheetTexture = new LTexture();
	if (success) {
		if (!AcidSpriteSheetTexture->loadFromFile( "sprites/Acid.png") ) {
			printf( "Failed to load acid animation texture!\n" );
			success = false;
		}
		else {
			// Set sprite clips
			for (int i = 0; i < ACID_ANIMATION_FRAMES; i++) {
				AcidSpriteClips[i].x = 20 * i;
				AcidSpriteClips[i].y = 0;
				AcidSpriteClips[i].w = 20;
				AcidSpriteClips[i].h = 20;
			}
		}
	}

	return success;
}

void Acid::render(int offsetX, int clipOffset) {
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);

	int renderX = offsetX + (posX - clipOffset);
	int rendLen = std::max(0, std::min(lenX, (posX - clipOffset) + lenX));
	if (rendLen == 0) return;

	for (int i = 0; i < lenX / 20; i++) {
		SDL_Rect* currentClip;
		if (renderX + i * 20 >= offsetX) currentClip = &AcidSpriteClips[((animationState + i * 2 * ACID_ANIMATION_UNIT) / ACID_ANIMATION_UNIT) % ACID_ANIMATION_FRAMES];
		else if (renderX + i * 20 + 20 > offsetX) {
			int atClip = ((animationState + i * 2 * ACID_ANIMATION_UNIT) / ACID_ANIMATION_UNIT) % ACID_ANIMATION_FRAMES;
			int intersection = (renderX + i * 20 + 20) - offsetX;
			currentClip = new SDL_Rect();

			currentClip->x = 20 * atClip + (20 - intersection);
			currentClip->y = 0;
			currentClip->w = intersection;
			currentClip->h = 20;
		}
		else continue;
		AcidSpriteSheetTexture->render(std::max(renderX + i * 20, offsetX), posY, currentClip);
	}

	animationState++;
	if (animationState == 4 * ACID_ANIMATION_UNIT) animationState = 0;

	SDL_Rect acidRect = {std::max(renderX, offsetX), posY + 20, rendLen, lenY - 20};
	SDL_RenderFillRect(gRenderer, &acidRect);
}