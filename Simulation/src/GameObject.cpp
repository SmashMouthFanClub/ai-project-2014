#include <iostream>
#include "GameObject.h"
#include "GameState.h"
#include "GameWorld.h"

GameObject::GameObject(GameWorld& gw, const ObjectPrototype& proto, double x, double y, double z) :
	m_meshName(proto.m_meshName), m_sceneEntity(nullptr), m_sceneNode(nullptr),
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
}

void GameObject::Update()
{
	Action a = {0.2, 0.5};	// dummy event, will use later
	dBodyAddForce(m_body, a.m_accelerateMagnitude * m_maxForward, 0, 0);
}

void GameObject::Render()
{
	const dReal *pos = dBodyGetPosition(m_body);
	const dReal *rot = dBodyGetQuaternion(m_body);
	m_sceneNode->setPosition(Ogre::Vector3(pos[0], pos[1], pos[2]));
	m_sceneNode->setOrientation(Ogre::Quaternion(rot[0], rot[1], rot[2], rot[3]));
}