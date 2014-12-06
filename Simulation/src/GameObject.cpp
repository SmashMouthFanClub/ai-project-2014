#include <iostream>
#include "GameObject.h"
#include "GameState.h"
#include "GameWorld.h"

const ObjectPrototype CarPrototype = {
	"CarBox",
	30, 40000, 10000,
	1000, 20,
	true, false, false
};

const ObjectPrototype PedPrototype = {
	"PedBox",
	180, 0, 0,
	70,	1,
	true, true, false
};

const ObjectPrototype BuildingPrototype = {
	"BuildingBox",
	0, 0, 0,
	1000000000, 1000000000, 
	false, true, true
};

GameObject::GameObject(GameWorld& gw, const ObjectPrototype& proto, double x, double y, double z) :
	m_meshName(proto.m_meshName), m_sceneEntity(nullptr), m_sceneNode(nullptr),	m_lockRotation(proto.m_lockRotation),
	m_maxTurn(proto.m_maxTurnAngle), m_maxForward(proto.m_maxForward), m_maxBackward(proto.m_maxBackward),
	m_hitPoints(proto.m_maxHitPoints)
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
	Action a = {0.3, 1};	// dummy event, will use later

	// calculate magnitude of force vector
	double forceMagnitude = a.m_accelerateMagnitude;
	if (forceMagnitude > 0)
		forceMagnitude *= m_maxForward;
	else
		forceMagnitude *= m_maxBackward;

	// calculate angle of force vector
	Ogre::Degree forceAngle(Ogre::Radian(a.m_turnMagnitude * m_maxTurn));

	// convert polar coordinates to x/y coordinates
	double forward = Ogre::Math::Cos(forceAngle) * forceMagnitude;
	double centripetal = Ogre::Math::Sin(forceAngle) * forceMagnitude;

	// get a rotation quaternion so we can do cool things with it
	const dReal *oldRotation = dBodyGetQuaternion(m_body);
	Ogre::Quaternion rotation = Ogre::Quaternion(oldRotation[0], oldRotation[1], oldRotation[2], oldRotation[3]);

	// apply forces
	Ogre::Vector3 force = rotation * Ogre::Vector3(forward, 0, centripetal);
	dBodySetForce(m_body, -1 * force[2], 0, force[0]);

	// set the rotation of the object
	Ogre::Vector3 angularVel = rotation * Ogre::Vector3(0, m_maxTurn * a.m_turnMagnitude * 0.1f, 0);
	dBodySetAngularVel(m_body, 0, angularVel[1], 0);
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
