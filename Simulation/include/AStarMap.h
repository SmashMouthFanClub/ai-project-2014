#ifndef _SIM_ASTARMAP_H
#define _SIM_ASTARMAP_H

#include <vector>
#include <map>
#include "Simulation.h"

class AStarMap {

	std::vector<WorldPos> m_nodes;
	std::map<int, std::vector<WorldPos>> m_adjacents;

public:

	AStarMap() = delete;
	AStarMap(const char *fileName);
	AStarMap(const AStarMap& copy) = delete;

	void makePath(WorldPos startPos, WorldPos endPos, std::vector<WorldPos>& outList);
	double getDistanceFromPath(WorldPos pos);

};

#endif