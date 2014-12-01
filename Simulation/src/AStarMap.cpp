#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include "GameState.h"
#include "AStarMap.h"

AStarMap::AStarMap(const char *fileName) {
	std::cout << fileName << std::endl;
}

void AStarMap::makePath(WorldPos startPos, WorldPos endPos, std::vector<WorldPos> &outList) {
     std::vector<Node *> explored;
     std::priority_queue<Node *, std::vector<Node *>, CompareNode> unexplored;
     double totalDist = getDistance(startPos, endPos);

     Node *start = new Node();
     start->parent = 0;
     start->pos = startPos;
     start->g = 0;
     start->h = totalDist;
     start->f = 0;

     unexplored.push(start);
     
     while (!unexplored.empty()) {
	  Node *top = unexplored.top();
	  int parent = getNodeIndex(top->pos);
	  std::vector<WorldPos> succ = m_adjacents[parent];

	  unexplored.pop();
	  
	  for (std::vector<WorldPos>::iterator it = succ.begin(); it != succ.end(); it++) {
	       if (getDistance(*it, top->pos) == 0) {
		    continue;
	       }
	       
	       bool ignore = false;
	       double h = getDistance(*it, endPos);
	       double g = top->g + getDistance(*it, top->pos);
		    
	       Node *child = new Node();
	       child->parent = top;
	       child->pos = *it;
	       child->g = g;
	       child->h = h;
	       child->f = g + h;
	       
	       if (h == 0) {
		    generatePath(child, outList);
		    return;
	       }

	       for (std::vector<Node *>::iterator check = explored.begin(); check != explored.end(); check++) {
		    if (getDistance((*check)->pos, child->pos) == 0) {
			 if ((*check)->f < child->f) {
			      ignore = true;
			      break;
			 }
		    }
	       }
	       
	       if (ignore) {
		    delete child;
		    continue;
	       }

	       unexplored.push(child);
	  }

	  explored.push_back(top);
     }
}

double AStarMap::getDistanceFromPath(WorldPos pos) {
	std::cout << "getDistanceFromPath()" << std::endl;
	return 1.0f;
}

double AStarMap::getDistance(WorldPos from, WorldPos to) {
     return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

int AStarMap::getNodeIndex(WorldPos node) {
     int index = 0;
     
     for (std::vector<WorldPos>::iterator it = m_nodes.begin(); it != m_nodes.end(); it++, index++) {
	  if ((*it).x == node.x && (*it).y == node.y) {
	       return index;
	  }
     }
     
     return -1;
}

void AStarMap::generatePath(Node *node, std::vector<WorldPos> &outList) {
     while (node != 0) {
	  outList.push_back(node->pos);
	  node = node->parent;
     }
     
     std::reverse(outList.begin(), outList.end());
}
