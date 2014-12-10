#ifndef _SIM_QLEARNINGMANAGER_H
#define _SIM_QLEARNINGMANAGER_H

#include <map>
#include "QLearningAgent.h"

class QLearningManager
{

	std::map<int, QLearningAgent*> m_agents;

public:

	~QLearningManager();

	QLearningAgent *GetAgent(int id);
	void SetExplore(double explore);

};

#endif