#ifndef _SIM_GAMEWORLD_H
#define _SIM_GAMEWORLD_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <OGRE/Ogre.h>

#include "GameObject.h"

class Game;

struct Camera
{
	Ogre::Camera *m_camera;
	Ogre::SceneNode *m_node;
};

class GameWorld
{

	Game& m_game;
	Ogre::SceneManager *m_scene;
	std::map<std::string, Ogre::Mesh*> m_meshes;
	std::vector<Camera> m_cameras;
	std::vector<GameObject> m_objects;

public:

	GameWorld(Game& game, std::string sceneName);

	bool Update();

	Ogre::SceneManager *GetScene();
	Ogre::Mesh *GetMesh(std::string meshName);

};

#endif