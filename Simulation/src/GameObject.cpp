#include <iostream>
#include "GameObject.h"
#include "GameState.h"
#include "GameWorld.h"

GameObject::GameObject(GameWorld& gw, std::string meshName, double turn, double forward, double backward) :
	m_mesh(meshName), m_maxTurn(turn), m_maxForward(forward), m_maxBackward(backward), m_x(0), m_y(0), m_z(0),
	m_sceneNode(nullptr), m_sceneEntity(nullptr)
{
	m_sceneEntity = gw.GetScene()->createEntity(gw.GetMesh(meshName));
	m_sceneNode = gw.GetScene()->getRootSceneNode()->createChildSceneNode();
	m_sceneNode->attachObject(m_sceneEntity);
}

void GameObject::SetX(double x)
{
	m_x = x;
}

void GameObject::SetY(double y)
{
	m_y = y;
}

void GameObject::SetZ(double z)
{
	m_z = z;
}

void GameObject::Update()
{
	Action a = {0.2, 0.5};	// dummy event, will use later
	m_x += 0.5f;
}

void GameObject::Render()
{
	m_sceneNode->setPosition(Ogre::Vector3(m_x, m_y, m_z));
}