#include <iostream>
#include "AStarMap.h"

int main(int argc, char **argv) {
	AStarMap a = {argv[1]};
	a.getDistanceFromPath(WorldPos(1, 1, 1, 1));

	return 0;
}