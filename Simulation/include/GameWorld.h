#ifndef _SIM_GAMEWORLD_H
#define _SIM_GAMEWORLD_H

#include <string>
#include <vector>
#include <map>
#include <OGRE/Ogre.h>

class Game;

class GameWorld {

	Game& m_game;
	std::map<std::string, Ogre::Mesh*> m_meshes;
	std::vector<Ogre::Camera*> m_cameras;

public:

	GameWorld(Game& game);

	bool Update();

};

#endif