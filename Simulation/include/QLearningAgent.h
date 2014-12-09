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
     double m_learn = 0;

     // The discount rate
     double m_discount = 0;

     // The exploration rate
     double m_explore = 0;

     // the weights for feature extraction
     double *m_weights = 0;

     // The qvalues
     std::map<int, std::vector<AVPair *> *> m_qvalues;
	  
public:
     QLearningAgent() = delete;
     QLearningAgent(double id, double learn, double discount, double explore, double *m_weights);
     QLearningAgent(const QLearningAgent& copy) = delete;
     ~QLearningAgent();
     
     void update(GameState current, Action action, GameState next, int reward);
     Action getAction(GameState state);
     int getValue(GameState state);
     double getExtracted(GameState state);
};

#endif
