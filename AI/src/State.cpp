#include "State.h"

Location *State::mainCarLocation()
{
     return m_mainCarLoc;
}

Location *State::adversaryCarLocation()
{
     return m_adversaryCarLoc;
}

Location *State::parkingSpotLocation()
{
     return m_parkingSpotLoc;
}

Location *State::getPedestrianLocations()
{
     return m_pedestrianLocs;
}

Location *State::getDumbCarLocations()
{
     return m_dumbCarLocs;
}
