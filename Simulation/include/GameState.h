#ifndef _SIM_GAMESTATE_H
#define _SIM_GAMESTATE_H

#include <vector>
#include "Simulation.h"

struct GameState {
	// vector of all nearby moving entities
	// z component of struct will be zero
	// all members are relative to the car
	std::vector<WorldPos> m_nearbyMoving;

	// vector of all nearby static entities (ie: buildings)
	// angle and z components of struct will be zero
	// all members are relative to the car
	std::vector<WorldPos> m_nearbyStatic;

	// distance of car from center of road, alway positive
	double m_distanceFromCenter;

	// distance of car from next destination
	double m_distanceFromDestination;

	// difference in car's angle and angle to next destination
	double m_deviationAngle;

	// damage done to self in last game tick
	int m_damageSelfInstant;

	// damage done to self in current episode
	int m_damageSelfTotal;

	// damage done to others in last game tick
	int m_damageOthersInstant;

	// damage done to others in current episode
	int m_damageOthersTotal;
};

#endif