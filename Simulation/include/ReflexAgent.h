#ifndef _SIM_REFLEXAGENT_H
#define _SIM_REFLEXAGENT_H

#include <map>
#include "GameState.h"

class ReflexAgent
{
     // The id of this agent
     int m_id = 0;

     // The rules governing which action to take at any given state
     std::map<GameState, Action> m_rules;
     
public:
     ReflexAgent() = delete;
     ReflexAgent(int id);
     ReflexAgent(const ReflexAgent& copy) = delete;

     // Add a rule to the rule map (or replace a current rule)
     void addRule(GameState state, Action action);

     // Get an action from a given state
     Action getAction(GameState state);
};

#endif
