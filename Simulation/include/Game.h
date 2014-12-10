#ifndef _SIM_GAME_H
#define _SIM_GAME_H

#include <memory>
#include <OGRE/Ogre.h>
#include "GameWorld.h"
#include "QLearningManager.h"

class Game
{

	// render stuff
	std::auto_ptr<Ogre::Root> m_root;
	Ogre::RenderWindow *m_window;
	Ogre::Viewport *m_viewport;

	// game stuff
	std::unique_ptr<GameWorld> m_gw;

	// other render stuff
	int m_framecount;
	int m_episode;

	// AI stuff
	QLearningManager m_manager;

public:

	Game(int w, int h);
	~Game();

	bool Tick();
	void SetCamera(Ogre::Camera *camera);

	int GetViewWidth();
	int GetViewHeight();

	void Reset();

	QLearningAgent *GetAgent(int id);

protected:

	Ogre::Root *GetRenderer();
	Ogre::RenderWindow *GetWindow();
	Ogre::Viewport *GetViewport();

	friend GameWorld;

};

#endif