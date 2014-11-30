#include <iostream>
#include "GameObject.h"
#include "GameState.h"
#include "GameWorld.h"

const ObjectPrototype CarPrototype = {
	"CarBox",
	30, 80000, 20000,
	1000, 20,
	true, false, false
};

const ObjectPrototype PedPrototype = {
	"PedBox",
	180, 80, 80,
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
	Action a = {0.8, 0.3};	// dummy event, will use later
	/*dBodySetForce(m_body, 0, 0, 0);
	dBodySetTorque(m_body, 0, 0, 0);
	dBodyAddRelForce(m_body, 0, 0, a.m_accelerateMagnitude * m_maxForward * 2);
	dBodyAddRelTorque(m_body, 0, a.m_accelerateMagnitude * m_maxForward * 8 , 0);*/

	const dReal *oldRotation = dBodyGetQuaternion(m_body);
	Ogre::Quaternion rotation = Ogre::Quaternion(oldRotation[0], oldRotation[1], oldRotation[2], oldRotation[3]);
	//Ogre::Vector3 force = rotation * Ogre::Vector3(0, 0, a.m_accelerateMagnitude * m_maxForward * 5);
	//Ogre::Vector3 torque = rotation * Ogre::Vector3(0, a.m_accelerateMagnitude * m_maxForward * 5, 0);
	Ogre::Vector3 linearVel = rotation * Ogre::Vector3(0, 0, 40);
	Ogre::Vector3 angularVel = rotation * Ogre::Vector3(0, 2, 0);

	//std::cout << force << std::endl;

	//const dReal *oldForce = dBodyGetForce(m_body);
	//dBodySetForce(m_body, force[0], oldForce[1], force[2]);

	const dReal *oldLinearVel = dBodyGetLinearVel(m_body);
	dBodySetLinearVel(m_body, linearVel[0], oldLinearVel[1], linearVel[2]);

	//const dReal *oldTorque = dBodyGetTorque(m_body);
	//dBodySetTorque(m_body, oldTorque[0], torque[1], oldTorque[2]);
	
	const dReal *oldAngularVel = dBodyGetAngularVel(m_body);
	dBodySetAngularVel(m_body, 0, angularVel[1], 0);

	if (m_lockRotation) {
		const dReal *oldRotation = dBodyGetRotation(m_body);
		dMatrix3 lockRotation;
		for (int i = 0; i < 12; ++i) {
			lockRotation[i] = oldRotation[i];
		}
		lockRotation[1] = 0;
		lockRotation[5] = 0.9f;
		lockRotation[9] = 0;
		dBodySetRotation(m_body, lockRotation);
	}
}

void GameObject::Render()
{
	const dReal *pos = dBodyGetPosition(m_body);
	const dReal *rot = dBodyGetQuaternion(m_body);
	m_sceneNode->setPosition(Ogre::Vector3(pos[0], pos[1], pos[2]));
	m_sceneNode->setOrientation(Ogre::Quaternion(rot[0], rot[1], rot[2], rot[3]));
}

dBodyID GameObject::GetPhysicsBody()
{
	return m_body;
}
