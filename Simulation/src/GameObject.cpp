#include <algorithm>
#include <iostream>
#include "GameObject.h"
#include "GameState.h"
#include "GameWorld.h"
#include "Game.h"

const ObjectPrototype CarPrototype = {
	"CarBox",
	100000, 80000, 20000,
	1000, 10,
	true, false, false,
	true
};

const ObjectPrototype PedPrototype = {
	"PedBox",
	0, 0, 0,
	70,	1,
	true, true, false,
	false
};

const ObjectPrototype Building1Prototype = {
	"Building1",
	0, 0, 0,
	1000000000, 1000000000, 
	false, true, true,
	false
};

const ObjectPrototype Building2Prototype = {
	"Building2",
	0, 0, 0,
	1000000000, 1000000000,
	false, true, true,
	false
};

const ObjectPrototype Building3Prototype = {
	"Building3",
	0, 0, 0,
	1000000000, 1000000000,
	false, true, true,
	false
};

const ObjectPrototype Building4Prototype = {
	"Building4",
	0, 0, 0,
	1000000000, 1000000000,
	false, true, true,
	false
};

GameObject::GameObject(GameWorld& gw, const ObjectPrototype& proto, double x, double y, double z, int id) :
	m_meshName(proto.m_meshName), m_sceneEntity(nullptr), m_sceneNode(nullptr),	m_lockRotation(proto.m_lockRotation), m_isKinematic(proto.m_isKinematic),
	m_maxTurn(proto.m_maxTurnAngle), m_maxForward(proto.m_maxForward), m_maxBackward(proto.m_maxBackward),
	m_hitPoints(proto.m_maxHitPoints), m_collisionAccum(0), m_totalDamage(0), m_hasAgent(proto.m_hasAgent), m_gw(gw), m_id(id), m_oldGS(nullptr), m_newGS(nullptr)
{
	m_sceneEntity = gw.GetScene()->createEntity(gw.GetMesh(m_meshName));
	m_sceneNode = gw.GetScene()->getRootSceneNode()->createChildSceneNode();
	m_sceneNode->attachObject(m_sceneEntity);

	Ogre::Vector3 size = m_sceneEntity->getBoundingBox().getSize();

	m_body = dBodyCreate(gw.GetPhysicsWorld());
	m_geom = dCreateBox(gw.GetPhysicsSpace(), size.x, size.y, size.z);
	dMassSetBox(&m_mass, proto.m_density, size.x, size.y, size.z);
	dBodySetMass(m_body, &m_mass);
	dGeomSetBody(m_geom, m_body);
	dBodySetPosition(m_body, x, y, z);
	
	// automagically disable things when the body is still for long enough
	dBodySetAutoDisableFlag(m_body, 1);
	dBodySetAutoDisableLinearThreshold(m_body, 0.1f);
	dBodySetAutoDisableAngularThreshold(m_body, 0.25f);
	dBodySetAutoDisableSteps(m_body, 1);

	// improve simulation accuracy
	dBodySetDamping(m_body, 0.0f, 0.1f);

	if (proto.m_registerCollisions) {
		gw.RegisterForCollisions(this);
	}

	if (proto.m_isKinematic) {
		dBodySetKinematic(m_body);
	}
}

void GameObject::Update()
{
     double reward = 0;
	if (!m_hasAgent) {
		return;
	}

	// reorder the game states
	if (m_oldGS != nullptr) {
		delete m_oldGS;
		m_oldGS = nullptr;
	}

	m_oldGS = m_newGS;
	m_newGS = new GameState;

	// construct the game state to pass to an agent
	GetGameState(*m_newGS);

	reward = 1 / std::max(1.d, m_newGS->m_distanceFromDestination);
	     
	// Discretize after reward calculation
	m_newGS->discretize();


	// get the agent for this object
	QLearningAgent *agent = m_gw.m_game.GetAgent(m_id);

	// Update the behaviors based on the last action
	if (m_oldGS != 0) {
	     agent->update(*m_oldGS, m_lastAction, *m_newGS, reward);
	}

	if (m_pathToDest.size() == 0) {
		Ogre::Vector3 a = GetLocation();
		WorldPos curr = {a.x, a.z, 0};
		m_gw.MakeMapPath(curr, m_pathToDest);
	}


	// here is where I'd pass the game state to an agent if we had one

	// layout: turn, acceleration
	m_lastAction = agent->getAction(*m_newGS);

	std::cout << "Action: " << m_lastAction.m_accelerateMagnitude << ", " << m_lastAction.m_turnMagnitude << std::endl;

	const dReal *quat = dBodyGetQuaternion(m_body);

	// do physics things
	if (quat[1] < 0.05f) {
		dBodySetForce(m_body, 0, 0, 0);
		if (m_lastAction.m_accelerateMagnitude > 0) {
			dBodyAddRelForce(m_body, 0, 0, m_lastAction.m_accelerateMagnitude * m_maxForward);
		} else {
			dBodyAddRelForce(m_body, 0, 0, m_lastAction.m_accelerateMagnitude * m_maxBackward);
		}
		dBodyAddTorque(m_body, 0, m_lastAction.m_turnMagnitude * m_maxTurn, 0);
	}

	// reset collision accumulator
	m_totalDamage += m_collisionAccum;
	m_collisionAccum = 0;
}

void GameObject::Render()
{
	const dReal *pos = dBodyGetPosition(m_body);
	const dReal *rot = dBodyGetQuaternion(m_body);
	m_sceneNode->setPosition(Ogre::Vector3(pos[0], pos[1], pos[2]));
	m_sceneNode->setOrientation(Ogre::Quaternion(rot[0], rot[1], rot[2], rot[3]));
}

Ogre::Vector3 GameObject::GetLocation()
{
	const dReal *pos = dBodyGetPosition(m_body);
	return Ogre::Vector3(pos[0], pos[1], pos[2]);
}

dBodyID GameObject::GetPhysicsBody()
{
	return m_body;
}

void GameObject::RegisterCollision(double depth)
{
	m_collisionAccum += depth;
}

void GameObject::GetGameState(GameState &gs)
{
	const dReal *velocity = dBodyGetLinearVel(m_body);
	gs.m_speed = sqrt(pow(velocity[0], 2) + pow(velocity[2], 2));

	// create vectors of differences between positions
	gs.m_nearbyMoving = std::vector<WorldPos>();
	gs.m_nearbyStatic = std::vector<WorldPos>();
	Ogre::Vector3 n = GetLocation();
	for (auto& i : m_gw.m_objects) {
		Ogre::Vector3 m = i.GetLocation();

		double diffX = m.x - n.x;
		double diffY = m.z - n.z;
		double theta = Ogre::Math::ATan2(diffY, diffX).valueDegrees();

		if (i.m_isKinematic) {
			gs.m_nearbyStatic.push_back(WorldPos {diffX, diffY, theta});
		} else {
			gs.m_nearbyMoving.push_back(WorldPos {diffX, diffY, theta});
		}
	}

	// this is a gross O(n) lookup of distance from a path
	WorldPos p = {n.x, n.z, 0};
	gs.m_distanceFromCenter = m_gw.m_map.getDistanceFromPath(p);

	// calculate the distance and angle to the nearest destination
	// this also removes destinations that you're very close to
	gs.m_distanceFromDestination = 0;
	gs.m_deviationAngle = 0;
	bool beginPopping = false;
	while ((m_pathToDest.size() != 0) && (gs.m_distanceFromDestination < 2.f)) {
		if (beginPopping) {
			m_pathToDest.pop_back();
		} else {
			beginPopping = true;
		}

		WorldPos p = m_pathToDest.back();

		double diffX = p.x - n.x;
		double diffY = p.y - n.z;

		gs.m_distanceFromDestination = Ogre::Math::Sqrt(Ogre::Math::Sqr(diffX) + Ogre::Math::Sqr(diffY));
		gs.m_deviationAngle = Ogre::Math::ATan2(diffY, diffX).valueDegrees();
	}

	// things that we've hit
	gs.m_damageSelfInstant = m_collisionAccum;
	gs.m_damageSelfTotal = m_totalDamage;
	gs.m_damageOthersInstant = 0; // yeah, not using this
	gs.m_damageOthersTotal = 0; // ... or this
}

void GameObject::GetReward(GameState& g1, GameState& g2)
{

}
