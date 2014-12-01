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

     // Create start node to add to the unexplored set
     Node *start = new Node();
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
		    
	       Node *child = new Node();
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
		    delete child;
		    continue;
	       }

	       unexplored.push(child);
	  }

	  // We have now explored the node we poped from the priority_queue
	  explored.push_back(top);
     }
}

double AStarMap::getDistanceFromPath(WorldPos pos) {
	std::cout << "getDistanceFromPath()" << std::endl;
	return 1.0f;
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
     
     std::reverse(outList.begin(), outList.end());
}
