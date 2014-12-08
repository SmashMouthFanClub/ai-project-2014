#ifndef _SIM_QLEARNING_H
#define _SIM_QLEARNING_H

#include <map>
#include <vector>
#include "GameState.h"

// Action reward pair
struct AVPair {
     Action action;
     int value;
};

// Compare AV pairs based on their value
class CompareAVPair {
public:
	bool operator()(AVPair  *av1, AVPair *av2)
	{
		return av1->value > av2->value;
	}
};

class QLearningAgent
{
     int m_id = 0;
     int m_learn = 0;
     int m_discount = 0;
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
