#ifndef _SIM_GAMEOBJECT_H
#define _SIM_GAMEOBJECT_H

#include <OGRE/Ogre.h>

class GameWorld;

class GameObject
{

	const std::string m_mesh;
	Ogre::Entity *m_sceneEntity;
	Ogre::SceneNode *m_sceneNode;

	const double m_maxTurn;
	const double m_maxForward;
	const double m_maxBackward;
	
	double m_x, m_y, m_z;

public:

	GameObject(GameWorld& gw, std::string meshName, double turn, double forward, double backward);

	void SetX(double x);
	void SetY(double y);
	void SetZ(double z);

	void Update();
	void Render();

};

#endif