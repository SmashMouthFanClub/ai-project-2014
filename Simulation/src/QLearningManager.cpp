#include "QLearningManager.h"

QLearningManager::~QLearningManager()
{
	for (auto i : m_agents) {
		delete i.second;
	}
}

QLearningAgent *QLearningManager::GetAgent(int id)
{
     // These are freed in the QLearningAgent
     double *weights = new double[4];
     weights[0] = 0.0; // closest object
     weights[1] = 0.0; // dest/speed or speed/dest
     weights[2] = 0.5; // distance to road
     weights[3] = 0.5; // deviation angle
     
	if (m_agents.count(id) == 0) {
	     m_agents[id] = new QLearningAgent(id, 0.5, 0.9, 0.1, weights);
	}

	return m_agents[id];
}

void QLearningManager::SetExplore(double explore)
{
	for (auto& i : m_agents) {
		i.second->setExploration(explore);
	}
}