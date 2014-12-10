#include <cstdlib>
#include "QLearningAgent.h"
#include "GameState.h"

QLearningAgent::QLearningAgent(double id, double learn, double discount, double explore, double *weights)
{
     // Set the member variables
     m_id = id;
     m_learn = learn;
     m_discount = discount;
     m_explore = explore;
     m_weights = weights;
}

QLearningAgent::~QLearningAgent()
{
     delete m_weights;
}

void QLearningAgent::update(GameState current, Action action, GameState next, int reward)
{
     // This function updates the QValues for the action that was just used
     std::vector<AVPair *> *pairs = m_qvalues[getExtracted(current)];
     
     for (AVPair *pair : *pairs) {
	  if (pair->action == action) {
	       pair->value = ((1 - m_learn) * pair->value) + (m_learn * (reward + (m_discount * getValue(next))));
	       break;
	  }
     }
}

Action QLearningAgent::getAction(GameState state)
{
     // This function returns the action with the highest value in a given state
     std::vector<AVPair *> *pairs = m_qvalues[getExtracted(state)];
     int val = 0;
     double rnd = 0;
     Action ret;
     
     for (AVPair *pair : *pairs) {
	  if (val <= pair->value) {
	       val = pair->value;
	       ret = pair->action;
	  }
     }

     rnd = rand() % 100;

     if ((rnd / 100) < m_explore) {
	  return (*pairs)[rand() % pairs->size()]->action;
     }

     return ret;
}

int QLearningAgent::getValue(GameState state)
{
     // This function returns the highest value for a given state
     std::vector<AVPair *> *pairs = m_qvalues[getExtracted(state)];
     int ret = 0;
     
     for (AVPair *pair : *pairs) {
	  if (ret <= pair->value) {
	       ret = pair->value;
	  }
     }

     return ret;
}

double QLearningAgent::getExtracted(GameState state)
{
     double ret = 0;
     std::vector<double> features;
     int index = 0;

     state.extractFeatures(features);

     for (std::vector<double>::iterator it = features.begin(); it != features.end(); it++, index++) {
	  ret += m_weights[index] * (*it);
     }
     
     return ret;
}
