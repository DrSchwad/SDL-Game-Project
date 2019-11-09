#include "Bullet.h"

// Stance animation
const int STANCE_ANIMATION_FRAMES = 4;
const int STANCE_ANIMATION_UNIT = 4;

// Running animation
const int RUNNING_ANIMATION_FRAMES = 15;
const int RUNNING_ANIMATION_UNIT = 2;

const int SHADOW_HEIGHT = 3, SHADOW_WIDTH = 20;
const int JUPM_UP_STATES = 13, JUMP_STILL_STATES = 5;
const int VERTICAL_VELOCITY = 8, HORIZONTAL_VELOCITY = 4;
const int FAST_DOWN_VELOCITY = 14, SLOW_DOWN_STATES = 10;

const int MAX_ANGLE = 60;

class Hero {
	public:
		Hero(bool _shootEnabled = false);
		void setObstacles(std::vector<CollisionArea> &_collisionAreas, std::vector<Block> &_blocks, std::vector<Acid> &_acids, std::vector<Spike> &_spikes);
		bool loadMedia();
		void setInitialPosition(int x, int y);
		int& getX();
		int& getY();
		void handleEvent(SDL_Event &e);
		void move(double &score);
		bool isStanding();
		bool isStanding(Block &b);
		void render(int offsetX, int clipOffset);
		void renderShadow(int offsetX, int clipOffset);
		void reset();
		void close();
		CollisionArea collisionArea;
		bool isDead;
		bool isLevelFinished;
	private:
		LTexture heroStanceRightSpriteSheetTexture;
		LTexture heroStanceLeftSpriteSheetTexture;
		LTexture heroRunningRightSpriteSheetTexture;
		LTexture heroRunningLeftSpriteSheetTexture;
		SDL_Rect heroStanceRightSpriteClips[STANCE_ANIMATION_FRAMES];
		SDL_Rect heroStanceLeftSpriteClips[STANCE_ANIMATION_FRAMES];
		SDL_Rect heroRunningRightSpriteClips[RUNNING_ANIMATION_FRAMES];
		SDL_Rect heroRunningLeftSpriteClips[RUNNING_ANIMATION_FRAMES];

		// Animation frame states
		int heroStanceFrame;
		int heroRunningFrame;

		int startX, startY;
		int posX, posY;
		int width, height;
		int collutionWidth, collisionHeight;

		bool rightPressed, leftPressed;
		int direction; // 0 for right, 1 for left
		int jumpState;
		int downState;

		int upVelocity, downVelocity;
		int leftVelocity, rightVelocity;

		std::vector<CollisionArea> collisionAreas;
		std::vector<Block> blocks;
		std::vector<Acid> acids;
		std::vector<Spike> spikes;
		std::vector<Bullet> bullets;

		// shoot mode
		bool shootEnabled;
		int angleState;
		double angle;
};

Hero::Hero(bool _shootEnabled) {
	heroStanceFrame = 0;
	heroRunningFrame = 0;

	posX = 0, posY = 0;
	width = 64, height = 64;
	collutionWidth = 20, collisionHeight = 64;

	rightPressed = leftPressed = false;
	direction = 0;
	jumpState = -1;
	downState = 0;

	upVelocity = leftVelocity = rightVelocity = 0;
	downVelocity = VERTICAL_VELOCITY;

	collisionArea = CollisionArea(RECTANGULAR_BOX, posX + (width - collutionWidth) / 2, posY, posX + (width + collutionWidth) / 2, posY + height);
	isDead = false;
	isLevelFinished = false;

	shootEnabled = _shootEnabled;
	angleState = 0;
}

void Hero::setObstacles(std::vector<CollisionArea> &_collisionAreas, std::vector<Block> &_blocks, std::vector<Acid> &_acids, std::vector<Spike> &_spikes) {
	collisionAreas = _collisionAreas;
	blocks = _blocks;
	acids = _acids;
	spikes = _spikes;
}

void Hero::setInitialPosition(int x, int y) {
	startX = posX = x;
	startY = posY = y;
}
int& Hero::getX() {
	return posX;
}
int& Hero::getY() {
	return posY;
}

bool Hero::loadMedia() {
	bool success = true;

	if (success) {
		if (!shootEnabled && !heroStanceRightSpriteSheetTexture.loadFromFile( "sprites/Hero Stance Right White.png") ) {
			printf( "Failed to load hero stance right animation texture!\n" );
			success = false;
		}
		if (shootEnabled && !heroStanceRightSpriteSheetTexture.loadFromFile( "sprites/Hero Shoot Stance Right White.png") ) {
			printf( "Failed to load hero stance right animation texture!\n" );
			success = false;
		}
		else {
			// Set sprite clips
			for (int i = 0; i < STANCE_ANIMATION_FRAMES; i++) {
				heroStanceRightSpriteClips[i].x = width * i;
				heroStanceRightSpriteClips[i].y = 0;
				heroStanceRightSpriteClips[i].w = width;
				heroStanceRightSpriteClips[i].h = height;
			}
		}
	}

	if (success) {
		if (!shootEnabled && !heroStanceLeftSpriteSheetTexture.loadFromFile( "sprites/Hero Stance Left White.png") ) {
			printf( "Failed to load hero stance left animation texture!\n" );
			success = false;
		}
		if (shootEnabled && !heroStanceLeftSpriteSheetTexture.loadFromFile( "sprites/Hero Shoot Stance Left White.png") ) {
			printf( "Failed to load hero stance left animation texture!\n" );
			success = false;
		}
		if (success) {
			// Set sprite clips
			for (int i = 0; i < STANCE_ANIMATION_FRAMES; i++) {
				heroStanceLeftSpriteClips[i].x = width * (STANCE_ANIMATION_FRAMES - 1 - i);
				heroStanceLeftSpriteClips[i].y = 0;
				heroStanceLeftSpriteClips[i].w = width;
				heroStanceLeftSpriteClips[i].h = height;
			}
		}
	}

	if (success) {
		if (!shootEnabled && !heroRunningRightSpriteSheetTexture.loadFromFile( "sprites/Hero Running Right White.png") ) {
			printf( "Failed to load hero running right animation texture!\n" );
			success = false;
		}
		if (shootEnabled && !heroRunningRightSpriteSheetTexture.loadFromFile( "sprites/Hero Shoot Running Right White.png") ) {
			printf( "Failed to load hero running right animation texture!\n" );
			success = false;
		}
		if (success) {
			// Set sprite clips
			for (int i = 0; i < RUNNING_ANIMATION_FRAMES; i++) {
				heroRunningRightSpriteClips[i].x = width * i;
				heroRunningRightSpriteClips[i].y = 0;
				heroRunningRightSpriteClips[i].w = width;
				heroRunningRightSpriteClips[i].h = height;
			}
		}
	}

	if (success) {
		if (!shootEnabled && !heroRunningLeftSpriteSheetTexture.loadFromFile( "sprites/Hero Running Left White.png") ) {
			printf( "Failed to load hero running left animation texture!\n" );
			success = false;
		}
		if (shootEnabled && !heroRunningLeftSpriteSheetTexture.loadFromFile( "sprites/Hero Shoot Running Left White.png") ) {
			printf( "Failed to load hero running left animation texture!\n" );
			success = false;
		}
		else {
			// Set sprite clips
			for (int i = 0; i < RUNNING_ANIMATION_FRAMES; i++) {
				heroRunningLeftSpriteClips[i].x = width * (RUNNING_ANIMATION_FRAMES - 1 - i);
				heroRunningLeftSpriteClips[i].y = 0;
				heroRunningLeftSpriteClips[i].w = width;
				heroRunningLeftSpriteClips[i].h = height;
			}
		}
	}

	return success;
}

void Hero::handleEvent(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == RIGHT_KEY_CODE) {
			if (!rightPressed) {
				rightPressed = true;
				heroStanceFrame = 0;
				heroRunningFrame = 0;
				direction = 0;
			}
		}
		else if (e.key.keysym.sym == LEFT_KEY_CODE) {
			if (!rightPressed && !leftPressed) {
				leftPressed = true;
				heroStanceFrame = 0;
				heroRunningFrame = 0;
				direction = 1;
			}
		}
		else if (e.key.keysym.sym == UP_KEY_CODE) {
			if (jumpState == -1 && isStanding()) {
				jumpState = 0;
				heroStanceFrame = 0;
				heroRunningFrame = 0;
			}
		}
		else if (e.key.keysym.sym == SHOOT_KEY_CODE) {
			if (shootEnabled) {
				int x = posX + width / 2;
				int y = posY + height / 3;
				int len = 20;
				double effectiveAngle;
				if (direction == 0) effectiveAngle = angle;
				else effectiveAngle = 180 - angle;
				effectiveAngle = effectiveAngle / 180 * acos(-1);
				int _x = x + cos(effectiveAngle) * len;
				int _y = y + sin(effectiveAngle) * len;

				bullets.push_back(Bullet(_x, _y, effectiveAngle));
			}
		}
	}

	if (e.type == SDL_KEYUP) {
		if (e.key.keysym.sym == RIGHT_KEY_CODE) rightPressed = false;
		else if (e.key.keysym.sym == LEFT_KEY_CODE) leftPressed = false;
	}
}

bool Hero::isStanding() {
	bool isStandingVar = false;
	CollisionArea c = collisionArea;
	c.topLeftY++, c.bottomRightY++;
	for (CollisionArea _c : collisionAreas) isStandingVar = isStandingVar || CollisionArea::doesCollide(c, _c);
	for (Block b : blocks) isStandingVar = isStandingVar || CollisionArea::doesCollide(c, b.collisionArea);
	return isStandingVar;
}

bool Hero::isStanding(Block &b) {
	bool isStandingVar = false;
	CollisionArea c = collisionArea;
	c.topLeftY++, c.bottomRightY++;
	for (CollisionArea _c : collisionAreas) isStandingVar = isStandingVar || CollisionArea::doesCollide(c, _c);
	isStandingVar = isStandingVar || CollisionArea::doesCollide(c, b.collisionArea);
	return isStandingVar;
}

void Hero::move(double &score) {
	if (shootEnabled) {
		angleState = (angleState + 2) % (4 * MAX_ANGLE);
		angle = angleState;
		if (angleState <= MAX_ANGLE) angle = angleState;
		else if (angleState <= 2 * MAX_ANGLE) angle = 2 * MAX_ANGLE - angleState;
		else if (angleState <= 3 * MAX_ANGLE) angle = 2 * MAX_ANGLE - angleState;
		else angle = angleState - 4 * MAX_ANGLE;

		for (Bullet &b : bullets) b.move(score, collisionAreas, blocks, acids, spikes);
		std::vector<Bullet> tmp = bullets;
		bullets.clear();
		for (Bullet &b : tmp) if (!b.isGone) bullets.push_back(b);
	}

	// If right arrow pressed, set right velocity
	rightVelocity = (rightPressed ? HORIZONTAL_VELOCITY : 0);

	// If right arrow not pressed, but left arrow pressed, set left velocity
	leftVelocity = ((!rightPressed && leftPressed) ? HORIZONTAL_VELOCITY : 0);

	// Handle jump velocity
	if (-1 < jumpState && jumpState <= JUPM_UP_STATES) { // If jumping upwards, set up velocity
		upVelocity = VERTICAL_VELOCITY * 2;
		jumpState++;
	}
	else if (JUPM_UP_STATES < jumpState && jumpState <= JUPM_UP_STATES + JUMP_STILL_STATES) { // If remaining still after upwards jump, set up velocity equal to down velocity
		upVelocity = VERTICAL_VELOCITY;
		jumpState++;
		if (jumpState > JUPM_UP_STATES + JUMP_STILL_STATES) jumpState = -1; // If remaining still period also ends, then set jump state to not jumping
	}
	else upVelocity = 0; // If not jumping, set up velocity to 0

	// If not jumping and not standing, increment down state
	if (jumpState == -1 && !isStanding()) downState++;
	else downState = 0;

	// If falling for a while, increment falling velocity
	if (downState >= SLOW_DOWN_STATES) downVelocity = FAST_DOWN_VELOCITY;
	else downVelocity = VERTICAL_VELOCITY;

	int upRemaining = upVelocity;
	int downRemaining = downVelocity;
	int leftRemaining = leftVelocity;
	int rightRemaining = rightVelocity;

	while (upRemaining || downRemaining || leftRemaining || rightRemaining) {
		int horizontalChange = 0, verticalChange = 0;

		if (upRemaining) verticalChange--, upRemaining--;
		if (downRemaining) verticalChange++, downRemaining--;
		if (leftRemaining) horizontalChange--, leftRemaining--;
		if (rightRemaining) horizontalChange++, rightRemaining--;

		posX += horizontalChange;
		collisionArea = {RECTANGULAR_BOX, posX + (width - collutionWidth) / 2, posY, posX + (width + collutionWidth) / 2, posY + height};
		bool collides = false;
		for (CollisionArea c : collisionAreas) collides = collides || CollisionArea::doesCollide(collisionArea, c);
		for (Block b : blocks) {
			if (CollisionArea::doesCollide(collisionArea, b.collisionArea)) {
				collides = true;
				isLevelFinished = isLevelFinished || b.finishingBlock;
			}
		}
		if (!collides) {
			for (Acid b : acids) collides = collides || CollisionArea::doesCollide(collisionArea, b.collisionArea);
			isDead = isDead || collides;
		}
		if (!collides) {
			for (Spike b : spikes) collides = collides || CollisionArea::doesCollide(collisionArea, b.collisionArea);
			isDead = isDead || collides;
		}
		if (collides) {
			posX -= horizontalChange;
			collisionArea = {RECTANGULAR_BOX, posX + (width - collutionWidth) / 2, posY, posX + (width + collutionWidth) / 2, posY + height};
		}

		posY += verticalChange;
		collisionArea = {RECTANGULAR_BOX, posX + (width - collutionWidth) / 2, posY, posX + (width + collutionWidth) / 2, posY + height};
		collides = false;
		for (CollisionArea c : collisionAreas) collides = collides || CollisionArea::doesCollide(collisionArea, c);
		for (Block b : blocks) {
			if (CollisionArea::doesCollide(collisionArea, b.collisionArea)) {
				collides = true;
				isLevelFinished = isLevelFinished || b.finishingBlock;
			}
		}
		if (collides) {
			posY -= verticalChange;
			collisionArea = {RECTANGULAR_BOX, posX + (width - collutionWidth) / 2, posY, posX + (width + collutionWidth) / 2, posY + height};
		}
	}
}

void Hero::render(int offsetX, int clipOffset) {
	if (!isStanding() || (!leftPressed && !rightPressed)) { // Jump or stance
		if (direction == 0) {
			SDL_Rect* currentClip = &heroStanceRightSpriteClips[heroStanceFrame / STANCE_ANIMATION_UNIT];
			heroStanceRightSpriteSheetTexture.render(offsetX + (posX - clipOffset), posY, currentClip);
		}
		else {
			SDL_Rect* currentClip = &heroStanceLeftSpriteClips[heroStanceFrame / STANCE_ANIMATION_UNIT];
			heroStanceLeftSpriteSheetTexture.render(offsetX + (posX - clipOffset), posY, currentClip);
		}

		//Go to next frame
		++heroStanceFrame;

		//Cycle animation
		if(heroStanceFrame / STANCE_ANIMATION_UNIT >= STANCE_ANIMATION_FRAMES) heroStanceFrame = 0;
	}
	else { // Running
		if (direction == 0) {
			SDL_Rect* currentClip = &heroRunningRightSpriteClips[heroRunningFrame / RUNNING_ANIMATION_UNIT];
			heroRunningRightSpriteSheetTexture.render(offsetX + (posX - clipOffset), posY, currentClip);
		}
		else {
			SDL_Rect* currentClip = &heroRunningLeftSpriteClips[heroRunningFrame / RUNNING_ANIMATION_UNIT];
			heroRunningLeftSpriteSheetTexture.render(offsetX + (posX - clipOffset), posY, currentClip);
		}

		//Go to next frame
		++heroRunningFrame;

		//Cycle animation
		if(heroRunningFrame / RUNNING_ANIMATION_UNIT >= RUNNING_ANIMATION_FRAMES) heroRunningFrame = 0;
	}

	if (shootEnabled) {
		int x = posX + width / 2;
		int y = posY + height / 3;
		int len = 20;
		double effectiveAngle;
		if (direction == 0) effectiveAngle = angle;
		else effectiveAngle = 180 - angle;
		effectiveAngle = effectiveAngle / 180 * acos(-1);
		int _x = x + cos(effectiveAngle) * len;
		int _y = y + sin(effectiveAngle) * len;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLine(gRenderer, offsetX + x, y, offsetX + _x, _y);

		for (Bullet &b : bullets) b.render(offsetX);
	}
}

void Hero::renderShadow(int offsetX, int clipOffset) {
	SDL_Rect shadowRect = {offsetX + (posX - clipOffset) + (width - SHADOW_WIDTH) / 2, posY + height - SHADOW_HEIGHT / 2, SHADOW_WIDTH, SHADOW_HEIGHT};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x55);
	SDL_RenderFillRect(gRenderer, &shadowRect);
}

void Hero::reset() {
	heroStanceFrame = 0;
	heroRunningFrame = 0;

	posX = startX, posY = startY;

	rightPressed = leftPressed = false;
	direction = 0;
	jumpState = -1;
	downState = 0;

	upVelocity = leftVelocity = rightVelocity = 0;
	downVelocity = VERTICAL_VELOCITY;

	collisionArea = CollisionArea(RECTANGULAR_BOX, posX + (width - collutionWidth) / 2, posY, posX + (width + collutionWidth) / 2, posY + height);
	isDead = false;
	isLevelFinished = false;
}

void Hero::close() {

}