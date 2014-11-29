#ifndef _SIM_GAMEOBJECT_H
#define _SIM_GAMEOBJECT_H

#include <OGRE/Ogre.h>
#include <ode/ode.h>

class GameWorld;

struct ObjectPrototype {
	std::string m_meshName;
	double m_maxTurnAngle;
	double m_maxForward;
	double m_maxBackward;
	double m_maxHitPoints;
	double m_density;
};

const ObjectPrototype CarPrototype = {
	"CarBox",
	30, 80000, 20000,
	1000,
	20
};

const ObjectPrototype PedPrototype = {
	"PedBox",
	180, 80, 80,
	70,
	1
};

const ObjectPrototype BuildingPrototype = {
	"BuildingBox",
	0, 0, 0,
	1000000000,
	1000000000
};

class GameObject
{
	// rendering stuff
	const std::string m_meshName;
	Ogre::Entity *m_sceneEntity;
	Ogre::SceneNode *m_sceneNode;

	// physics stuff
	dBodyID m_body;
	dGeomID m_geom;
	dMass m_mass;

	// game sim stuff
	const double m_maxTurn;
	const double m_maxForward;
	const double m_maxBackward;
	double m_hitPoints;

public:

	GameObject(GameWorld& gw, const ObjectPrototype& proto, double x, double y, double z);

	void Update();
	void Render();

};

#endif