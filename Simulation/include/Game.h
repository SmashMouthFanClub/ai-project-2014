#ifndef _SIM_GAME_H
#define _SIM_GAME_H

#include <memory>
#include <OGRE/Ogre.h>
#include "GameWorld.h"

class Game
{

	std::auto_ptr<Ogre::Root> m_root;
	Ogre::RenderWindow *m_window;
	Ogre::Viewport *m_viewport;

	std::unique_ptr<GameWorld> m_gw;

	int m_framecount;

public:

	Game(int w, int h);
	~Game();

	bool Tick();
	void SetCamera(Ogre::Camera *camera);

	int GetViewWidth();
	int GetViewHeight();

	void Reset();

protected:

	Ogre::Root *GetRenderer();
	Ogre::RenderWindow *GetWindow();
	Ogre::Viewport *GetViewport();

	friend GameWorld;

};

#endif