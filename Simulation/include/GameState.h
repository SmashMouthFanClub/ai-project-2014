#ifndef _SIM_GAMESTATE_H
#define _SIM_GAMESTATE_H

#include <vector>

// relative coordinates and angle of other game objects
struct WorldPos {
	// carteesian x and y coords
	double x, y;

	// angle in degrees because they're easier for people to reason about
	double angle;
};

struct GameState {
	// vector of all nearby moving entities
	// all members are relative to the car
	// x = abs(self.x - other.x)
	// y = abs(self.y - other.y)
	// angle = self.angle - other.angle
	std::vector<WorldPos> m_nearbyMoving;

	// vector of all nearby static entities (ie: buildings)
	// angle component of struct will be zero
	// others are as above in m_nearbyMoving
	std::vector<WorldPos> m_nearbyStatic;

	// distance of car from center of road, alway positive
	// image a capsule (rectangle w/ semicircles on either end) around a line straight down a road
	// m_distanceFromCenter is the radius of the semicircles that would put the car or pedestrian on
	// the perimeter of that capsule.
	double m_distanceFromCenter;

	// distance of car from next destination
	// this is a euclidean distance and can't be negative
	double m_distanceFromDestination;

	// difference in car's angle and angle to next destination
	// ex: car is 154 degrees from origin along positive x axis, goal is 90, m_deviationAngle is 64
	// ex: car is 90 degrees from origin along positive x axis, goal is 154, m_deviationAngle is -64
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

struct Action {

	// -1 <= tMag <= 1, -1 is a left turn, 1 is a right turn, clipped to range of turning radius of given object
	// for example, pedestrians have a turn radius of 90 degrees, so they can turn left or right by 90 degrees
	// per game frame.  cars might only have a turning radius of 10, so they can turn at max 10 degrees either way.
	double m_turnMagnitude;

	// -1 <= aMag <= 1, -1 is backwards, 1 is forward, -1 to 0 is clipped to the reverse speed and 0 to 1 is clipped
	// to the maximum forward speed.  for example, some car might have a range from -20 to 80, which means that -1
	// is 20 MPH backwards, 0 is 0 MPH, 1 is 80 MPH, -0.5 is -10 MPH, 0.5 is 40 MPH, etc... while pedestrians might
	// have a range from -1 to 1 MPH
	double m_accelerateMagnitude;

	// any given game object can perform a turn and an accelerate in one game tick.  braking is applying a negative
	// acceleration, or waiting for friction to do its magic.  cars and pedestrians have different frictions - people
	// can stop on a dime, cars don't.
};

#endif