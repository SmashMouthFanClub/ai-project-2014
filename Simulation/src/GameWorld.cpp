#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "Game.h"
#include "GameWorld.h"
#include "AStarMap.h"

Ogre::MeshPtr CreateBox(Ogre::SceneManager *scene, Ogre::String name, double w, double h, double d, Ogre::ColourValue color1, Ogre::ColourValue color2);
Ogre::MeshPtr CreatePlane(Ogre::SceneManager *scene, Ogre::String name, double x, double y, Ogre::ColourValue color);

void NearCollideCallback(void *data, dGeomID o1, dGeomID o2);

GameWorld::GameWorld(Game& game, std::string sceneName) :
	m_game(game), m_map("paths.txt"), m_scene(nullptr)
{
	m_scene = game.GetRenderer()->createSceneManager(Ogre::ST_GENERIC, sceneName);

	// Important game objects
	m_meshes["CarBox"] = CreateBox(m_scene, "CarBox", 2.0f, 1.5f, 4.0f, Ogre::ColourValue(0.25f, 0.25f, 0.80f), Ogre::ColourValue(0.15f, 0.15f, 0.5f));
	m_meshes["PedBox"] = CreateBox(m_scene, "PedBox", 0.5f, 2.0f, 0.5f, Ogre::ColourValue(0.80f, 0.25f, 0.25f), Ogre::ColourValue(0.5f, 0.15f, 0.15f));
	m_meshes["Ground"] = CreatePlane(m_scene, "Ground", 5000, 5000, Ogre::ColourValue(0.25f, 0.70f, 0.25f));

	// Assorted buildings
	m_meshes["Building1"] = CreateBox(m_scene, "Building1", 10.0f, 100.0f, 10.0f, Ogre::ColourValue(0.5f, 0.5f, 0.5f), Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	m_meshes["Building2"] = CreateBox(m_scene, "Building2", 20.0f, 100.0f, 20.0f, Ogre::ColourValue(0.4f, 0.4f, 0.4f), Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	m_meshes["Building3"] = CreateBox(m_scene, "Building3", 20.0f, 100.0f, 10.0f, Ogre::ColourValue(0.3f, 0.3f, 0.3f), Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	m_meshes["Building4"] = CreateBox(m_scene, "Building4", 10.0f, 100.0f, 20.0f, Ogre::ColourValue(0.5f, 0.5f, 0.5f), Ogre::ColourValue(0.3f, 0.3f, 0.3f));

	// Assorted roads
	m_meshes["RoadH1"] = CreatePlane(m_scene, "RoadH1",  60, 10, Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	m_meshes["RoadH2"] = CreatePlane(m_scene, "RoadH2", 110, 10, Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	m_meshes["RoadH3"] = CreatePlane(m_scene, "RoadH3", 160, 10, Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	m_meshes["RoadV1"] = CreatePlane(m_scene, "RoadV1", 10,  60, Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	m_meshes["RoadV2"] = CreatePlane(m_scene, "RoadV2", 10, 110, Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	m_meshes["RoadV3"] = CreatePlane(m_scene, "RoadV3", 10, 160, Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	// create the ground
	Ogre::Entity *ground = m_scene->createEntity(GetMesh("Ground"));
	Ogre::SceneNode *groundNode = m_scene->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(ground);
	groundNode->setPosition(Ogre::Vector3(0, 0, 0));

	// create physics stuff
	m_world = dWorldCreate();
	m_space = dHashSpaceCreate(0);
	m_group = dJointGroupCreate(0);
	dWorldSetGravity(m_world, 0, -10, 0);
	dWorldSetCFM(m_world, 1e-5);
	dCreatePlane(m_space, 0, 1, 0, 0);

	// dictionary of prototypes
	std::map<std::string, const ObjectPrototype*> protos;
	protos["Car"] = &CarPrototype;
	protos["Ped"] = &PedPrototype;
	protos["Building1"] = &Building1Prototype;
	protos["Building2"] = &Building2Prototype;
	protos["Building3"] = &Building3Prototype;
	protos["Building4"] = &Building4Prototype;

	// load game objects
	std::ifstream inFile("objects.txt");
	int id = 0;
	while (!inFile.eof()) {
		std::string line;
		std::getline(inFile, line);
		std::istringstream is(line);

		std::string name;
		int isObject;
		double x, y, z;

		is >> name;
		is >> isObject;
		is >> x;
		is >> y;
		is >> z;

		if (isObject == 0) {
			m_statics.push_back(StaticMesh(*this, name, Ogre::Vector3(x, y, z)));
		} else {
			m_objects.push_back(GameObject(*this, *protos[name], x, y, z, id));
			++id;
		}
	}

	// create a camera
	Ogre::Camera *camera = m_scene->createCamera("PrimaryCamera");
	camera->setNearClipDistance(2.0f);
	camera->setFarClipDistance(1000.f);
	//Ogre::SceneNode *cameraNode = m_scene->getRootSceneNode()->createChildSceneNode("PrimaryCameraNode");
	Ogre::SceneNode *cameraNode = m_objects.front().m_sceneNode->createChildSceneNode("PrimaryCameraNode");
	cameraNode->attachObject(camera);
	m_cameras.push_back(Camera {camera, cameraNode});
	game.SetCamera(camera);
	cameraNode->setPosition(Ogre::Vector3(0, 30, -50));
	camera->lookAt(Ogre::Vector3(0, 10, 0));

	// test code plz ignore
	/*std::vector<WorldPos> path;
	WorldPos w1 = {0, 0, 0};
	WorldPos w2 = {300, 300, 0};
	m_map.makePath(w1, w2, path);*/
}

GameWorld::~GameWorld()
{
	dSpaceDestroy(m_space);
	dWorldDestroy(m_world);

	m_game.GetRenderer()->destroySceneManager(m_scene);
}

bool GameWorld::Update()
{
	for (auto& object : m_objects) {
		object.Update();
	}

	// simulate physics
	dSpaceCollide(m_space, (void*) this, &NearCollideCallback);
	dWorldStep(m_world, 1.f / 60.f);
	dJointGroupEmpty(m_group);

	//Ogre::Vector3 objPos = m_objects.front().GetLocation();
	//m_cameras.front().m_camera->lookAt(objPos);

	for (auto& object : m_objects) {
		object.Render();
	}

	return true;
}

AStarMap& GameWorld::GetMap()
{
	return m_map;
}

void GameWorld::MakeMapPath(WorldPos start, std::vector<WorldPos>& outList)
{
	int len = m_map.getSize();
	int idx1 = m_map.getClosestNode(start);
	int idx2 = rand() % (len - 1);

	if (idx2 >= idx1) {
		idx2 += 1;
	}

	WorldPos p1, p2;
	m_map.getNode(idx1, p1);
	m_map.getNode(idx2, p2);

	std::cout << "(" << p1.x << ", " << p1.y << ")" << std::endl;
	std::cout << "(" << p2.x << ", " << p2.y << ")" << std::endl;

	m_map.makePath(p1, p2, outList);
}

void GameWorld::NearCollideCallback(void *data, dGeomID o1, dGeomID o2)
{
	GameWorld *self = (GameWorld*) data;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	dContact contact;

	contact.surface.mode = dContactBounce | dContactApprox1;
	contact.surface.mu = 0.7;
	contact.surface.bounce = 0;
	contact.surface.bounce_vel = 0.1;

	int numcol = dCollide(o1, o2, 1, &contact.geom, sizeof(dContact));

	if (numcol > 0) {
		dJointID j = dJointCreateContact(self->m_world, self->m_group, &contact);
		dJointAttach(j, b1, b2);

		if (contact.geom.depth < 0.5) {
			return;
		}

		if (self->m_bodyToObject.find(b1) != self->m_bodyToObject.end()) {
			GameObject *obj = self->m_bodyToObject[b1];
			obj->RegisterCollision(contact.geom.depth * 3.f);
		}

		if (self->m_bodyToObject.find(b2) != self->m_bodyToObject.end()) {
			GameObject *obj = self->m_bodyToObject[b2];
			obj->RegisterCollision(contact.geom.depth * 3.f);
		}
	}
}

Ogre::SceneManager *GameWorld::GetScene()
{
	return m_scene;
}

Ogre::MeshPtr GameWorld::GetMesh(std::string meshName)
{
	return m_meshes[meshName];
}

dWorldID GameWorld::GetPhysicsWorld()
{
	return m_world;
}

dSpaceID GameWorld::GetPhysicsSpace()
{
	return m_space;
}

void GameWorld::RegisterForCollisions(GameObject *body)
{
	m_bodyToObject[body->GetPhysicsBody()] = body;
}

Ogre::MeshPtr CreateBox(Ogre::SceneManager *scene, Ogre::String name, double w, double h, double d, Ogre::ColourValue color1, Ogre::ColourValue color2)
{
	Ogre::ManualObject *temp = scene->createManualObject(name);
	temp->setDynamic(false);

	temp->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	{
		temp->position(-w,  h, -d);
		temp->colour(color1);
		temp->position( w,  h, -d);
		temp->colour(color1);
		temp->position( w, -h, -d);
		temp->colour(color2);
		temp->position(-w, -h, -d);
		temp->colour(color2);

		temp->position(-w,  h,  d);
		temp->colour(color1);
		temp->position( w,  h,  d);
		temp->colour(color1);
		temp->position( w, -h,  d);
		temp->colour(color2);
		temp->position(-w, -h,  d);
		temp->colour(color2);

		temp->triangle(0,1,2);
		temp->triangle(2,3,0);
		temp->triangle(4,6,5);
		temp->triangle(6,4,7);
		temp->triangle(0,4,5);
		temp->triangle(5,1,0);
		temp->triangle(2,6,7);
		temp->triangle(7,3,2);
		temp->triangle(0,7,4);
		temp->triangle(7,0,3);
		temp->triangle(1,5,6);
		temp->triangle(6,2,1);	
	}
	temp->end();
	return temp->convertToMesh(name);
}

Ogre::MeshPtr CreatePlane(Ogre::SceneManager *scene, Ogre::String name, double x, double y, Ogre::ColourValue color)
{
	Ogre::ManualObject *temp = scene->createManualObject(name);
	temp->setDynamic(false);

	temp->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	{
		temp->position( x,  0,  y);
		temp->colour(color);
		temp->position(-x,  0,  y);
		temp->colour(color);
		temp->position(-x,  0, -y);
		temp->colour(color);
		temp->position( x,  0, -y);
		temp->colour(color);

		temp->triangle(0,2,1);
		temp->triangle(0,3,2);
	}
	temp->end();
	return temp->convertToMesh(name);
}
