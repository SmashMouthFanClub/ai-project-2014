#include "ReflexAgent.h"
#include "GameState.h"

ReflexAgent::ReflexAgent(int id)
{
     m_id = id;
}

void ReflexAgent::addRule(GameState state, Action action)
{
     m_rules[state] = action;
}

Action ReflexAgent::getAction(GameState state)
{
     return m_rules[state];
}
