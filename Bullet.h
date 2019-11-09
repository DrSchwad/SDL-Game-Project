class Bullet {
	public:
		Bullet(int posX, int posY, double angle);
		void move(double &score, std::vector<CollisionArea> &_collisionAreas, std::vector<Block> &_blocks, std::vector<Acid> &_acids, std::vector<Spike> &_spikes);
		void render(int offsetX);
		bool isGone;
	private:
		int posX, posY;
		double angle;
		CollisionArea collisionArea;
};

Bullet::Bullet(int posX, int posY, double angle) : posX(posX), posY(posY), angle(angle) {
	isGone = false;
	collisionArea = {RECTANGULAR_BOX, posX - 2, posY - 2, posX + 2, posY + 2};
}

void Bullet::move(double &score, std::vector<CollisionArea> &collisionAreas, std::vector<Block> &blocks, std::vector<Acid> &acids, std::vector<Spike> &spikes) {
	double dX = 5;
	double dY = tan(angle) * dX;
	posX += dX;
	posY += dY;
	collisionArea = {RECTANGULAR_BOX, posX - 2, posY - 2, posX + 2, posY + 2};
	score = 0;

	bool collides = false;
	for (CollisionArea &c : collisionAreas) collides = collides || CollisionArea::doesCollide(collisionArea, c);
	for (Block &b : blocks) {
		if (CollisionArea::doesCollide(collisionArea, b.collisionArea)) {
			collides = true;
			if (b.finishingBlock) score += 10;
		}
	}
	if (!collides) {
		for (Acid &b : acids) collides = collides || CollisionArea::doesCollide(collisionArea, b.collisionArea);
	}
	if (!collides) {
		for (Spike &b : spikes) collides = collides || CollisionArea::doesCollide(collisionArea, b.collisionArea);
	}
	if (collides) {
		isGone = true;
	}
}

void Bullet::render(int offsetX) {
	if (!isGone) {
		SDL_Rect indicatorRect = {offsetX + posX - 2, posY - 2, 4, 4};
	SDL_RenderFillRect(gRenderer, &indicatorRect);
	}
}