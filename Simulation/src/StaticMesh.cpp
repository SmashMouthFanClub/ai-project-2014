#include "StaticMesh.h"
#include "GameWorld.h"

StaticMesh::StaticMesh(GameWorld& gw, std::string meshName, Ogre::Vector3 position) :
	m_meshName(meshName), m_position(position), m_gw(gw)
{
	m_sceneEntity = gw.GetScene()->createEntity(gw.GetMesh(m_meshName));
	m_sceneNode = gw.GetScene()->getRootSceneNode()->createChildSceneNode();
	m_sceneNode->attachObject(m_sceneEntity);
	m_sceneNode->setPosition(position);
}

StaticMesh::~StaticMesh()
{

}