#include "GameState.h"

bool operator==(const Action &act1, const Action &act2)
{
     return act1.m_turnMagnitude == act2.m_turnMagnitude && act1.m_accelerateMagnitude == act2.m_accelerateMagnitude;
}

bool operator<(const GameState &state1, const GameState &state2)
{
     // This is useless since map order does not matter, but its needed
     return state1.m_damageSelfTotal < state2.m_damageSelfTotal;
}
