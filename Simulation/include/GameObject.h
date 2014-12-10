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
	bool m_hasAgent;
};

extern const ObjectPrototype CarPrototype;
extern const ObjectPrototype PedPrototype;
extern const ObjectPrototype Building1Prototype;
extern const ObjectPrototype Building2Prototype;
extern const ObjectPrototype Building3Prototype;
extern const ObjectPrototype Building4Prototype;

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
	bool m_hasAgent;
	std::vector<WorldPos> m_pathToDest;
	GameWorld& m_gw;
	int m_id;

	GameState *m_oldGS;
	GameState *m_newGS;
     Action m_lastAction;

public:

	GameObject(GameWorld& gw, const ObjectPrototype& proto, double x, double y, double z, int id);

	void Update();
	void Render();

	Ogre::Vector3 GetLocation();

	dBodyID GetPhysicsBody();
	void RegisterCollision(double depth);

private:

	void GetGameState(GameState &gs);
	void GetReward(GameState& g1, GameState& g2);

	friend GameWorld;

};

#endif
