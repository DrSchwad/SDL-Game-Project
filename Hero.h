//Stance animation
const int STANCE_ANIMATION_FRAMES = 4;
SDL_Rect heroStanceSpriteClips[ STANCE_ANIMATION_FRAMES ];
LTexture heroStanceSpriteSheetTexture;
const int SHADOW_HEIGHT = 3, SHADOW_WIDTH = 20;

class Hero {
	public:
		Hero();
		bool loadMedia();
		void handleEvent(SDL_Event &e);
		void render(int offsetX, int groundHeight);
		void renderShadow();
		void close();
	private:
		int heroStanceFrame;
		int posX, posY;
		int width, height;
};

Hero::Hero() {
	heroStanceFrame = 0;
}

bool Hero::loadMedia() {
	bool success = true;
	if (!heroStanceSpriteSheetTexture.loadFromFile( "sprites/Hero Stance White.png") ) {
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else {
		width = heroStanceSpriteSheetTexture.getWidth() / STANCE_ANIMATION_FRAMES;
		height = heroStanceSpriteSheetTexture.getHeight();

		// Set sprite clips
		for (int i = 0; i < STANCE_ANIMATION_FRAMES; i++) {
			heroStanceSpriteClips[i].x = width * i;
			heroStanceSpriteClips[i].y = 0;
			heroStanceSpriteClips[i].w = width;
			heroStanceSpriteClips[i].h = height;
		}
	}
	return success;
}

void Hero::handleEvent(SDL_Event &e) {

}

void Hero::render(int offsetX, int groundHeight) {
	SDL_Rect* currentClip = &heroStanceSpriteClips[ heroStanceFrame / 4 ];
	heroStanceSpriteSheetTexture.render( offsetX + 40, SCREEN_HEIGHT - groundHeight - currentClip->h, currentClip );

	//Go to next frame
    ++heroStanceFrame;

    //Cycle animation
    if( heroStanceFrame / 4 >= STANCE_ANIMATION_FRAMES ) heroStanceFrame = 0;

    posX = offsetX + 40, posY = SCREEN_HEIGHT - groundHeight - currentClip->h;
}

void Hero::renderShadow() {
	SDL_Rect shadowRect = { posX + (width - SHADOW_WIDTH) / 2, posY + height - SHADOW_HEIGHT / 2, SHADOW_WIDTH, SHADOW_HEIGHT };
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect( gRenderer, &shadowRect );
}

void Hero::close() {

}