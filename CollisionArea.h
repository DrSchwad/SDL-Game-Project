enum BoxType {
	RECTANGULAR_BOX,
	CIRCULAR_BOX
};

class CollisionArea {
	public:
		CollisionArea();
		CollisionArea(BoxType _boxType, int v1, int v2, int v3, int v4 = 0, bool _isDeadly = false);

		BoxType boxType;

		int topLeftX, topLeftY;
		int bottomRightX, bottomRightY;

		int centerX, centerY;
		int radius;

		bool isDeadly;

		static bool doesCollide(CollisionArea &c1, CollisionArea &c2);
		// void render(int offsetX) {
		// 	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		// 	SDL_Rect blockRect = {offsetX + topLeftX, topLeftY, bottomRightX - topLeftX, bottomRightY - topLeftY};
		// 	SDL_RenderFillRect(gRenderer, &blockRect);
		// }
	private:
		static int distanceSquared(int x1, int y1, int x2, int y2);
		static bool doesCollideRR(CollisionArea &c1, CollisionArea &c2);
		static bool doesCollideRC(CollisionArea &c1, CollisionArea &c2);
		static bool doesCollideCC(CollisionArea &c1, CollisionArea &c2);
};

CollisionArea::CollisionArea() {}
CollisionArea::CollisionArea(BoxType _boxType, int v1, int v2, int v3, int v4, bool _isDeadly) {
	boxType = _boxType;
	if (_boxType == RECTANGULAR_BOX) {
		topLeftX = v1;
		topLeftY = v2;
		bottomRightX = v3;
		bottomRightY = v4;
	}
	else {
		centerX = v1;
		centerY = v2;
		radius = v3;
	}
	isDeadly = _isDeadly;
}

bool CollisionArea::doesCollide(CollisionArea &c1, CollisionArea &c2) {
	if (c1.boxType == RECTANGULAR_BOX && c2.boxType == RECTANGULAR_BOX) return doesCollideRR(c1, c2);
	if (c1.boxType == RECTANGULAR_BOX && c2.boxType == CIRCULAR_BOX) return doesCollideRC(c1, c2);
	if (c1.boxType == CIRCULAR_BOX && c2.boxType == RECTANGULAR_BOX) return doesCollideRC(c2, c1);
	else return doesCollideCC(c1, c2);
}

int CollisionArea::distanceSquared(int x1, int y1, int x2, int y2) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool CollisionArea::doesCollideRR(CollisionArea &c1, CollisionArea &c2) {
	// The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	// Calculate the sides of rect A
	leftA = c1.topLeftX;
	rightA = c1.bottomRightX;
	topA = c1.topLeftY;
	bottomA = c1.bottomRightY;

	// Calculate the sides of rect B
	leftB = c2.topLeftX;
	rightB = c2.bottomRightX;
	topB = c2.topLeftY;
	bottomB = c2.bottomRightY;

	// If any of the sides from A are outside of B
	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;

	// If none of the sides from A are outside B
	return true;
}

bool CollisionArea::doesCollideRC(CollisionArea &c1, CollisionArea &c2) {
	// Closest point on collision box
	int cX, cY;

	// Find closest x offset
	if (c2.centerX < c1.topLeftX) cX = c1.topLeftX;
	else if (c2.centerX > c1.bottomRightX) cX = c1.bottomRightX;
	else cX = c2.centerX;

	// Find closest y offset
	if (c2.centerY < c1.topLeftY) cY = c1.topLeftY;
	else if (c2.centerY > c1.bottomRightY) cY = c1.bottomRightY;
	else cY = c2.centerY;

	// If the closest point is inside the circle
	if (distanceSquared(c2.centerX, c2.centerY, cX, cY) < c2.radius * c2.radius) return true;

	// If the shapes have not collided
	return false;
}

bool CollisionArea::doesCollideCC(CollisionArea &c1, CollisionArea &c2) {
	// Calculate total radius squared
	int totalRadiusSquared = c1.radius + c2.radius;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	// If the distance between the centers of the circles is less than the sum of their radii
	if (distanceSquared(c1.centerX, c1.centerY, c2.centerX, c2.centerY) < totalRadiusSquared) return true;

	// If not
	return false;
}