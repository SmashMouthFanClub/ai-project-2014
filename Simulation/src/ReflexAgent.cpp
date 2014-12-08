#include "ReflexAgent.h"
#include "GameState.h"

ReflexAgent::ReflexAgent(int id)
{
     // Set the agent id
     m_id = id;
}

void ReflexAgent::addRule(GameState state, Action action)
{
     // Set the rule for state -> action
     m_rules[valueExtraction(state)] = action;
}

Action ReflexAgent::getAction(GameState state)
{
     // Return an action for a given state
     return m_rules[valueExtraction(state)];
}
