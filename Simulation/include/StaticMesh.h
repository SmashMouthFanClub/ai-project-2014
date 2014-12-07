#ifndef _SIM_STATIC_MESH_H
#define _SIM_STATIC_MESH_H

#include <OGRE/Ogre.h>

class GameWorld;

class StaticMesh
{
	// rendering stuff
	const std::string m_meshName;
	Ogre::Entity *m_sceneEntity;
	Ogre::SceneNode *m_sceneNode;

	// position
	Ogre::Vector3 m_position;

	// sim stuff
	GameWorld& m_gw;

public:

	StaticMesh(GameWorld& gw, std::string meshName, Ogre::Vector3 positon);
	~StaticMesh();

};

#endif