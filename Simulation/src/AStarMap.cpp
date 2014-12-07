#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

#include "GameState.h"
#include "AStarMap.h"

double LineSegmentDistance(WorldPos l1, WorldPos l2, WorldPos p);

AStarMap::AStarMap(const char *fileName)
{
	std::ifstream inFile(fileName);

	while (!inFile.eof()) {
		std::string line;
		std::getline(inFile, line);

		//if (line.size() < 2) continue;
		if (line == "#") break;

		std::istringstream is(line);
		double id, x, y;
		is >> id;
		is >> x;
		is >> y;

		WorldPos w = {x, y, 0};
		m_nodes.push_back(w);
	}

	while (!inFile.eof()) {
		std::string line;
		std::getline(inFile, line);

		//if (line.size() < 2) continue;

		std::istringstream is(line);
		double n1, n2;
		is >> n1;
		is >> n2;

		if (m_adjacents.find(n1) == m_adjacents.end()) {
			m_adjacents[n1] = std::vector<WorldPos>();
		}

		if (m_adjacents.find(n2) == m_adjacents.end()) {
			m_adjacents[n2] = std::vector<WorldPos>();
		}

		WorldPos w1 = m_nodes[n1];
		WorldPos w2 = m_nodes[n2];

		m_adjacents[n1].push_back(w2);
		m_adjacents[n2].push_back(w1);
	}

	for (auto& i : m_adjacents) {
		WorldPos w = m_nodes[i.first];
		std::cout << "(" << w.x << ", " << w.y << "): ";
		for (auto& j : i.second) {
			std::cout << "(" << j.x << ", " << j.y << "); ";
		}
		std::cout << std::endl << std::endl;
	}
}

void AStarMap::makePath(WorldPos startPos, WorldPos endPos, std::vector<WorldPos> &outList) {
     std::vector<Node *> explored;
     std::priority_queue<Node *, std::vector<Node *>, CompareNode> unexplored;
     double totalDist = getDistance(startPos, endPos);

	 std::vector<Node> nodePool;

     // Create start node to add to the unexplored set
     nodePool.push_back(Node());
     Node *start = &nodePool.back();
     start->parent = 0;
     start->pos = startPos;
     start->g = 0;
     start->h = totalDist;
     start->f = 0;

     // Add start node to unexplored set
     unexplored.push(start);

     // Loop until unexplored is empty
     while (!unexplored.empty()) {
	  // Get the top of the unexplored set. Note: unexplored set is a priority queue
	  // It uses CompareNode(n1, n2) = n1.f < n2.f to set up the queue, so top should be the node
	  // with the lowest 'f' value
	  Node *top = unexplored.top();
	  int parent = getNodeIndex(top->pos);
	  std::vector<WorldPos> succ = m_adjacents[parent];

	  unexplored.pop();

	  // Loop through the node's successors, excluding the node itself
	  for (std::vector<WorldPos>::iterator it = succ.begin(); it != succ.end(); it++) {
	       if (getDistance(*it, top->pos) == 0) {
		    continue;
	       }
	       
	       bool ignore = false;

	       // Calculate the h and g values and set up a child node for the sets
	       double h = getDistance(*it, endPos);
	       double g = top->g + getDistance(*it, top->pos);
		   
		   nodePool.push_back(Node());
	       Node *child = &nodePool.back();
	       child->parent = top;
	       child->pos = *it;
	       child->g = g;
	       child->h = h;
	       child->f = g + h;

	       // If h (distance to endPos (goal)) is 0, then this is the goal state, so we return
	       if (h == 0) {
		    generatePath(child, outList);
		    return;
	       }

	       // If a node exists with the same position and a lower 'f' value then don't add this one to the unexplored set
	       for (std::vector<Node *>::iterator check = explored.begin(); check != explored.end(); check++) {
		    if (getDistance((*check)->pos, child->pos) == 0) {
			 if ((*check)->f < child->f) {
			      ignore = true;
			      break;
			 }
		    }
	       }

	       // If we are not adding this one to the unexplored set, then delete it
	       if (ignore) {
		    //delete child;
		    continue;
	       }

	       unexplored.push(child);
	  }

	  // We have now explored the node we poped from the priority_queue
	  explored.push_back(top);
     }
}

double AStarMap::getDistanceFromPath(WorldPos pos) {
	double minDist = 100000.0f;
	for (auto& i : m_adjacents) {
		WorldPos p = m_nodes[i.first];

		for (auto& j : i.second) {
			double dist = LineSegmentDistance(p, j, pos);
			if (dist < minDist) {
				minDist = dist;
			}
		}
	}

	return minDist;
}

double LineSegmentDistance(WorldPos l1, WorldPos l2, WorldPos p) {
	double lengthSquared = pow(l1.x - l2.x, 2) + pow(l1.y - l2.y, 2);
	if (lengthSquared < 0.0001f) {
		return sqrt(pow(l1.x - p.x, 2) + pow(l1.y - p.y, 2));
	}

	WorldPos x1 = {p.x - l1.x, p.y - l1.y, 0};
	WorldPos x2 = {l2.x - l1.x, l2.y - l1.y, 0};
	double scalarProjection = ((x1.x * x2.x) + (x1.y * x2.y)) / lengthSquared;

	if (scalarProjection < 0.0f) {
		return sqrt(pow(l1.x - p.x, 2) + pow(l1.y - p.y, 2));
	}

	if (scalarProjection > 1.0f) {
		return sqrt(pow(l2.x - p.x, 2) + pow(l2.y - p.y, 2));
	}

	WorldPos vectorProjection = {
		l1.x + scalarProjection * l2.x - scalarProjection * l1.x,
		l1.y + scalarProjection * l2.y - scalarProjection * l1.y,
		0
	};
	return sqrt(pow(vectorProjection.x - p.x, 2) - pow(vectorProjection.y - p.y, 2));
}

double AStarMap::getDistance(WorldPos from, WorldPos to) {
     // Return the distance between two WorldPos
     return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

int AStarMap::getNodeIndex(WorldPos node) {
     // This function returns the index of a WorldPos in the m_nodes member of this class
     // I assume the int key in the m_adjacents is this index
     int index = 0;
     
     for (std::vector<WorldPos>::iterator it = m_nodes.begin(); it != m_nodes.end(); it++, index++) {
	  if ((*it).x == node.x && (*it).y == node.y) {
	       return index;
	  }
     }
     
     return -1;
}

void AStarMap::generatePath(Node *node, std::vector<WorldPos> &outList) {
     // Tnis function converts the Node linked list into a vector, and reverses it
     // so that the startPos is at the first item and endPos is the last item.
     while (node != 0) {
	  outList.push_back(node->pos);
	  node = node->parent;
     }
     
     // it's easier to work with this in reverse
     //std::reverse(outList.begin(), outList.end());
}
