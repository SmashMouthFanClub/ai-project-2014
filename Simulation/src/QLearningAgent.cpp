#include <iostream>
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
     double rnd = 0;
     int val = 0;
     Action ret;

     std::cout << "Map: " << getExtracted(state) << " ";

     if (pairs == NULL) {
	  pairs = new std::vector<AVPair *>();
	  m_qvalues[getExtracted(state)] = pairs;
     }

     rnd = ((rand() % 100) / 100.d);

     if (rnd < m_explore || pairs->size() == 0) {
	  std::cout << "exploring!" << std::endl;
	  
	  ret = (Action) { .m_turnMagnitude = truncate(((rand() % 100) / 100.d) * (rand() % 10 < 5 ? -1 : 1)),
			   .m_accelerateMagnitude = truncate(((rand() % 100) / 100.d) * (rand() % 10 < 5 ? -1 : 1)) };

	  AVPair *pair = new AVPair { .action = ret, .value = 0 };
	  
	  pairs->push_back(pair);
	  
	  return ret;
     }

     val = ((*pairs)[0])->value;
     
     for (AVPair *pair : *pairs) {
	  if (val <= pair->value) {
	       val = pair->value;
	       ret = pair->action;
	  }
     }

     std::cout << "not exploring! " << std::endl;

     return ret;
}

int QLearningAgent::getValue(GameState state)
{
     // This function returns the highest value for a given state
     std::vector<AVPair *> *pairs = m_qvalues[getExtracted(state)];
     int ret = 0;

     if (pairs == NULL) {
	  pairs = new std::vector<AVPair *>();
	  m_qvalues[getExtracted(state)] = pairs;
     }
     
     for (AVPair *pair : *pairs) {
	  if (ret <= pair->value) {
	       ret = pair->value;
	  }
     }

     return ret;
}

int QLearningAgent::getExtracted(GameState state)
{
     double ret = 0;
     std::vector<double> features;
     int index = 0;

     state.extractFeatures(features);

     for (std::vector<double>::iterator it = features.begin(); it != features.end(); it++, index++) {
	  ret += m_weights[index] * (*it);
     }
     
     return ret * 10.d;
}

double QLearningAgent::truncate(double in) {
     return (((int) (in * 10) % 5) * 2) / 10.d;
}

void QLearningAgent::setExploration(double explore) {
     m_explore = explore;
}