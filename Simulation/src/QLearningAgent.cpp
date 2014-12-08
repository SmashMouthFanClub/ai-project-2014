#include "QLearningAgent.h"
#include "GameState.h"

QLearningAgent::QLearningAgent(int id, int learn, int discount)
{
     m_id = id;
     m_learn = learn;
     m_discount = discount;
}

void QLearningAgent::update(GameState current, Action action, GameState next, int reward)
{
     std::vector<AVPair *> *pairs = m_qvalues[current];
     
     for (AVPair *pair : *pairs) {
	  if (pair->action == action) {
	       pair->value = ((1 - m_learn) * pair->value) + (m_learn * (reward + (m_discount * getValue(next))));
	       break;
	  }
     }
}

Action QLearningAgent::getAction(GameState state)
{
     std::vector<AVPair *> *pairs = m_qvalues[state];
     int val = 0;
     Action ret;
     
     for (AVPair *pair : *pairs) {
	  if (val <= pair->value) {
	       val = pair->value;
	       ret = pair->action;
	  }
     }

     return ret;     
}

int QLearningAgent::getValue(GameState state)
{
     std::vector<AVPair *> *pairs = m_qvalues[state];
     int ret = 0;
     
     for (AVPair *pair : *pairs) {
	  if (ret <= pair->value) {
	       ret = pair->value;
	  }
     }

     return ret;
}
