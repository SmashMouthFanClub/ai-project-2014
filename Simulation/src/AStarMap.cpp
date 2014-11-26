#include <iostream>
#include "AStarMap.h"

AStarMap::AStarMap(const char *fileName) {
	std::cout << fileName << std::endl;
}

void AStarMap::makePath(WorldPos startPos, WorldPos endPos, std::vector<WorldPos> &outList) {
	std::cout << "makePath()" << std::endl;
}

double AStarMap::getDistanceFromPath(WorldPos pos) {
	std::cout << "getDistanceFromPath()" << std::endl;
	return 1.0f;
}