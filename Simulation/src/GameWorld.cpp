#include <iostream>

#include "Game.h"
#include "GameWorld.h"

GameWorld::GameWorld(Game& game, std::string sceneName) : m_game(game), m_scene(nullptr)
{
	m_scene = game.GetRenderer()->createSceneManager(Ogre::ST_GENERIC, sceneName);\

	for (int i = 0; i < 10; ++i) {
		m_objects.push_back(GameObject("A", 30, 80, 20));
	}
}

bool GameWorld::Update()
{
	std::cout << "Beginning Tick" << std::endl;

	for (auto object : m_objects) {
		object.Update();
	}

	// simulate physics

	for (auto object : m_objects) {
		object.Render();
	}

	return true;
}