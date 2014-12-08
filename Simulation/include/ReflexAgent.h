#ifndef _SIM_REFLEXAGENT_H
#define _SIM_REFLEXAGENT_H

#include <map>
#include "GameState.h"

class ReflexAgent
{
     int m_id = 0;
     std::map<GameState, Action> m_rules;
     
public:
     ReflexAgent(int id);
     void addRule(GameState state, Action action);
     Action getAction(GameState state);
};

#endif
