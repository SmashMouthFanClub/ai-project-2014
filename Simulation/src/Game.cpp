#include <cstdlib>
#include <iostream>
#include <ode/ode.h>
#include "Game.h"

Game::Game(int w, int h) :
	m_window(nullptr), m_viewport(nullptr), m_framecount(0)
{
	// create OGRE root object
	m_root = std::auto_ptr<Ogre::Root>(new Ogre::Root("plugins.cfg", "", "logs"));

	// initialize physics
	dInitODE();

	// set the root's render system
	const Ogre::RenderSystemList& renderSystems = m_root->getAvailableRenderers();
	if (renderSystems.size() == 0) {
		Ogre::LogManager::getSingleton().logMessage("No render systems found");
		exit(1);
	}
	m_root->setRenderSystem(renderSystems[0]);
	m_root->initialise(false, "", "");

	// create a window
	Ogre::NameValuePairList params;
	params["FSAA"] = "0";
	params["vsync"] = "true";
	params["border"] = "fixed";
	m_window = m_root->createRenderWindow("Test Window", w, h, false, &params);
	m_window->setActive(true);
	m_window->setAutoUpdated(false);

	m_gw = std::unique_ptr<GameWorld>(new GameWorld(*this, "GameWorld"));
}

Game::~Game()
{
	dCloseODE();
}

bool Game::Tick()
{
	if (!m_gw->Update()) {
		return false;
	}

	++m_framecount;

	if (m_framecount > 3000) {
		Reset();
		m_framecount = 0;
	}

	m_window->update(true);
	//m_window->swapBuffers();

	m_root->renderOneFrame();

	Ogre::WindowEventUtilities::messagePump();
	return !m_window->isClosed();
}

void Game::SetCamera(Ogre::Camera *camera)
{
	if (m_viewport == nullptr) {
		m_viewport = m_window->addViewport(camera);
		camera->setAspectRatio(GetViewWidth() / GetViewHeight());
		m_viewport->setAutoUpdated(true);
		m_viewport->setBackgroundColour(Ogre::ColourValue(0.3, 0.3, 1));
	} else {
		m_viewport->setCamera(camera);
	}
}

int Game::GetViewWidth()
{
	if (m_viewport != nullptr) {
		return m_viewport->getActualWidth();
	}
	return -1;
}

void Game::Reset()
{
	delete m_gw.release();
	m_gw = std::unique_ptr<GameWorld>(new GameWorld(*this, "GameWorld"));
}

int Game::GetViewHeight()
{
	if (m_viewport != nullptr) {
		return m_viewport->getActualHeight();
	}
	return -1;
}

Ogre::Root *Game::GetRenderer()
{
	return m_root.get();
}

Ogre::RenderWindow *Game::GetWindow()
{
	return m_window;
}