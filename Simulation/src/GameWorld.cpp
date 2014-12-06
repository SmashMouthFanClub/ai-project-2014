#include <iostream>

#include "Game.h"
#include "GameWorld.h"
#include "AStarMap.h"

Ogre::MeshPtr CreateBox(Ogre::SceneManager *scene, Ogre::String name, double w, double h, double d, Ogre::ColourValue color1, Ogre::ColourValue color2);
Ogre::MeshPtr CreatePlane(Ogre::SceneManager *scene, Ogre::String name, double x, double y, Ogre::ColourValue color);

void NearCollideCallback(void *data, dGeomID o1, dGeomID o2);

GameWorld::GameWorld(Game& game, std::string sceneName) : m_game(game), m_scene(nullptr)
{
	m_scene = game.GetRenderer()->createSceneManager(Ogre::ST_GENERIC, sceneName);

	m_meshes["CarBox"] = CreateBox(m_scene, "CarBox", 2.0f, 1.5f, 4.0f, Ogre::ColourValue(0.25f, 0.25f, 0.80f), Ogre::ColourValue(0.15f, 0.15f, 0.5f));
	m_meshes["PedBox"] = CreateBox(m_scene, "PedBox", 0.5f, 2.0f, 0.5f, Ogre::ColourValue(0.80f, 0.25f, 0.25f), Ogre::ColourValue(0.5f, 0.15f, 0.15f));
	m_meshes["Ground"] = CreatePlane(m_scene, "Ground", 500, 500, Ogre::ColourValue(0.25f, 0.70f, 0.25f));
	m_meshes["Building1"] = CreateBox(m_scene, "Building1", 10.0f, 20.0f, 10.0f, Ogre::ColourValue(0.5f, 0.5f, 0.5f), Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	// create a camera
	Ogre::Camera *camera = m_scene->createCamera("PrimaryCamera");
	camera->setNearClipDistance(2.0f);
	camera->setFarClipDistance(1000.f);
	Ogre::SceneNode *cameraNode = m_scene->getRootSceneNode()->createChildSceneNode("PrimaryCameraNode");
	cameraNode->attachObject(camera);
	m_cameras.push_back(Camera {camera, cameraNode});
	game.SetCamera(camera);
	cameraNode->setPosition(Ogre::Vector3(-100, 100, -100));
	camera->lookAt(Ogre::Vector3(0, 0, 0));

	// create the ground
	Ogre::Entity *ground = m_scene->createEntity(GetMesh("Ground"));
	Ogre::SceneNode *groundNode = m_scene->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(ground);
	groundNode->setPosition(Ogre::Vector3(0, 0, 0));

	// create physics stuff
	m_world = dWorldCreate();
	m_space = dHashSpaceCreate(0);
	m_group = dJointGroupCreate(0);
	dWorldSetGravity(m_world, 0, -9.81f, 0);
	dWorldSetCFM(m_world, 1e-5);
	dCreatePlane(m_space, 0, 1, 0, 0);

	// single test car
	//m_objects.push_back(GameObject(*this, CarPrototype, 0, 2, 0));

	// create some cars
	for (int i = 0; i < 1; ++i) {
		m_objects.push_back(GameObject(*this, CarPrototype, (i % 5) * 10.f, 2, (i / 5) * 10.f));
	}

	// create some pedestrians
	for (int i = 0; i < 10; ++i) {
		m_objects.push_back(GameObject(*this, PedPrototype, i * 5, 2, 45 - i * 5));
	}

	m_objects.push_back(GameObject(*this, BuildingPrototype, 50, 20, 50));
	m_objects.push_back(GameObject(*this, BuildingPrototype, 100, 20, 50));
	m_objects.push_back(GameObject(*this, BuildingPrototype, 50, 20, 100));
	m_objects.push_back(GameObject(*this, BuildingPrototype, 100, 20, 100));

	AStarMap astar("paths.txt");
	std::vector<WorldPos> path;
	WorldPos w1 = {0, 0, 0};
	WorldPos w2 = {300, 300, 0};
	astar.makePath(w1, w2, path);

	for (WorldPos& i : path) {
		std::cout << i.x << ", " << i.y << std::endl;
	}
}

GameWorld::~GameWorld()
{
	//dSpaceDestroy(m_space);
	//dWorldDestroy(m_world);
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

	Ogre::Vector3 objPos = m_objects.front().GetLocation();
	m_cameras.front().m_camera->lookAt(objPos);

	for (auto& object : m_objects) {
		object.Render();
	}

	return true;
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
