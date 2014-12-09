#ifndef _SIM_GAMEWORLD_H
#define _SIM_GAMEWORLD_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <OGRE/Ogre.h>
#include <ode/ode.h>

#include "GameObject.h"
#include "StaticMesh.h"
#include "AStarMap.h"

class Game;

struct Camera
{
	Ogre::Camera *m_camera;
	Ogre::SceneNode *m_node;
};

class GameWorld
{
	// game data stuff
	Game& m_game;
	std::vector<GameObject> m_objects;
	std::vector<StaticMesh> m_statics;
	AStarMap m_map;

	// rendering member variables
	Ogre::SceneManager *m_scene;
	std::map<std::string, Ogre::MeshPtr> m_meshes;
	std::vector<Camera> m_cameras;

	// physics member variables
	dWorldID m_world;
	dSpaceID m_space;
	dJointGroupID m_group;
	std::map<dBodyID, GameObject*> m_bodyToObject;

public:

	GameWorld(Game& game, std::string sceneName);
	~GameWorld();

	bool Update();

	AStarMap& GetMap();
	void MakeMapPath(WorldPos start, std::vector<WorldPos>& outList);

	Ogre::SceneManager *GetScene();
	Ogre::MeshPtr GetMesh(std::string meshName);

	dWorldID GetPhysicsWorld();
	dSpaceID GetPhysicsSpace();
	void RegisterForCollisions(GameObject *body);

	static void NearCollideCallback(void *data, dGeomID o1, dGeomID o2);

	friend GameObject;
};

#endif