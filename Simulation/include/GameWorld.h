#ifndef _SIM_GAMEWORLD_H
#define _SIM_GAMEWORLD_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <OGRE/Ogre.h>

#include "GameObject.h"

class Game;

class GameWorld
{

	Game& m_game;
	Ogre::SceneManager *m_scene;
	std::map<std::string, Ogre::Mesh*> m_meshes;
	std::vector<Ogre::Camera*> m_cameras;
	std::vector<GameObject> m_objects;

public:

	GameWorld(Game& game, std::string sceneName);

	bool Update();

};

#endif