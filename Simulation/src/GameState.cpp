#include <algorithm>
#include <cmath>
#include <iostream>
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

double discretizeLinear(double in, double interval)
{
	double discrete = trunc(trunc(in / interval) * interval);
	//std::cout << "LIN: " << in << " -> " << discrete << std::endl;
	return discrete;
}

double discretizeLogarithmic(double in)
{
	double sign = 1;
	in = trunc(in);

	if (in < 1) {
		in = 1;
		sign = -1;
	}

	double discrete = trunc(exp2(trunc(log2(in))));
	discrete *= sign;
	std::cout << "LOG: " << in << " -> " << discrete << std::endl;
	return discrete;
}

void GameState::discretize()
{
	// linear discretized features:
	// * m_nearbyMoving[i].angle
	// * m_nearbyStatic[i].angle
	// * m_deviationAngle
	// * m_damageSelfInstant
	// * m_damageSelfTotal


	// logarithmic features:
	// * m_nearbyMoving[i].x
	// * m_nearbyMoving[i].y
	// * m_nearbyStatic[i].x
	// * m_nearbyStatic[i].y
	// * m_distanceFromCenter
	// * m_distanceFromDestination

	// non discretized features:
	// * m_damageOthersInstant
	// * m_damageOthersTotal

	for (auto& i : m_nearbyMoving) {
		i.x = discretizeLogarithmic(i.x);
		i.y = discretizeLogarithmic(i.y);
		i.angle = discretizeLinear(i.angle, 40);
	}

	for (auto& i : m_nearbyStatic) {
		i.x = discretizeLogarithmic(i.x);
		i.y = discretizeLogarithmic(i.y);
		i.angle = discretizeLinear(i.angle, 40);
	}

	m_distanceFromCenter = discretizeLogarithmic(m_distanceFromCenter);
	m_distanceFromDestination = discretizeLogarithmic(m_distanceFromDestination);
	m_deviationAngle = discretizeLinear(m_deviationAngle, 40);

	m_damageSelfInstant = discretizeLinear(m_damageSelfInstant, 5);
	m_damageSelfTotal = discretizeLinear(m_damageSelfTotal, 10);
}

void GameState::extractFeatures(std::vector<double> &ret)
{
     double closest = 0;
     double dist = 0;
     double m_carVelocity = 0;
     
     for (auto &cur : m_nearbyMoving) {
	  if (cur.angle < 25 && cur.angle > -25) {
	       dist = sqrt(pow(cur.x, 2) + pow(cur.y, 2));

	       if (dist < closest) {
		    closest = dist;
	       }
	  }
     }

     for (auto &cur : m_nearbyStatic) {
	  if (cur.angle < 25 && cur.angle > -25) {
	       dist = sqrt(pow(cur.x, 2) + pow(cur.y, 2));

	       if (dist < closest) {
		    closest = dist;
	       }
	  }
     }

     ret.push_back(closest);
     ret.push_back(std::min(std::max(m_distanceFromDestination, 1.d) / std::max(m_carVelocity, 1.d), std::max(m_carVelocity, 1.d) / std::max(m_distanceFromDestination, 1.d)));
}
