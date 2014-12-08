#ifndef _SIM_QLEARNING_H
#define _SIM_QLEARNING_H

#include <map>
#include <vector>
#include "GameState.h"

// Action value pair
struct AVPair {
     Action action;
     int value;
};

// QLearning agent
class QLearningAgent
{
     // The Id of the agent
     int m_id = 0;

     // The learning rate
     int m_learn = 0;

     // The discount rate
     int m_discount = 0;

     // The qvalues
     std::map<GameState, std::vector<AVPair *> *> m_qvalues;
	  
public:
     QLearningAgent() = delete;
     QLearningAgent(int id, int learn, int discount);
     QLearningAgent(const QLearningAgent& copy) = delete;

     void update(GameState current, Action action, GameState next, int reward);
     Action getAction(GameState state);
     int getValue(GameState state);
};

#endif
