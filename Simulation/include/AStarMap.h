#ifndef _SIM_ASTARMAP_H
#define _SIM_ASTARMAP_H

#include <vector>
#include <map>

struct Node {
	struct Node *parent;
	WorldPos pos;
	double f, g, h;
};

class CompareNode {
public:
	bool operator()(Node  *n1, Node *n2)
	{
		return n1->f < n2->f;
	}
};

class AStarMap {

	std::vector<WorldPos> m_nodes;
	std::map<int, std::vector<WorldPos>> m_adjacents;

public:

	AStarMap() = delete;
	AStarMap(const char *fileName);
	AStarMap(const AStarMap& copy) = delete;

	void makePath(WorldPos startPos, WorldPos endPos, std::vector<WorldPos>& outList);
	double getDistanceFromPath(WorldPos pos);
	double getDistance(WorldPos from, WorldPos to);
	int getNodeIndex(WorldPos node);
	void generatePath(Node *node, std::vector<WorldPos> &outList);
};

#endif
