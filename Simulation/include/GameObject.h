#ifndef _SIM_GAMEOBJECT_H
#define _SIM_GAMEOBJECT_H

#include <OGRE/Ogre.h>
#include <ode/ode.h>

#include "GameState.h"

class GameWorld;

struct ObjectPrototype {
	std::string m_meshName;
	double m_maxTurnAngle;
	double m_maxForward;
	double m_maxBackward;
	double m_maxHitPoints;
	double m_density;
	bool m_registerCollisions;
	bool m_lockRotation;
	bool m_isKinematic;
};

extern const ObjectPrototype CarPrototype;
extern const ObjectPrototype PedPrototype;
extern const ObjectPrototype BuildingPrototype;

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
	bool m_lockRotation;
	bool m_isKinematic;

	// game sim stuff
	const double m_maxTurn;
	const double m_maxForward;
	const double m_maxBackward;
	double m_hitPoints;
	double m_collisionAccum;
	double m_totalDamage;
	std::vector<WorldPos> m_pathToDest;
	GameWorld& m_gw;

public:

	GameObject(GameWorld& gw, const ObjectPrototype& proto, double x, double y, double z);

	void Update();
	void Render();

	Ogre::Vector3 GetLocation();

	dBodyID GetPhysicsBody();
	void RegisterCollision(double depth);

};

#endif