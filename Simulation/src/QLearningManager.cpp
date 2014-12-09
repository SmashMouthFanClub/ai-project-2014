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
     
	if (m_agents.count(id) == 0) {
	     m_agents[id] = new QLearningAgent(id, 0.5f, 0.9f, 0.2f, weights);
	}

	return m_agents[id];
}
