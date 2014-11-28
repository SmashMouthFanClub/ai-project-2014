#include <iostream>

#include "Game.h"
#include "GameWorld.h"

Ogre::MeshPtr CreateBox(Ogre::SceneManager *scene, Ogre::String name, double w, double h, double d, Ogre::ColourValue color);

GameWorld::GameWorld(Game& game, std::string sceneName) : m_game(game), m_scene(nullptr)
{
	m_scene = game.GetRenderer()->createSceneManager(Ogre::ST_GENERIC, sceneName);

	m_meshes["CarBox"] = CreateBox(m_scene, "CarBox", 2.0f, 1.5f, 4.0f, Ogre::ColourValue(0.25f, 0.25f, 0.80f));
	m_meshes["PedBox"] = CreateBox(m_scene, "PedBox", 0.5f, 2.0f, 0.5f, Ogre::ColourValue(0.80f, 0.25f, 0.25f));

	// create a camera
	Ogre::Camera *camera = m_scene->createCamera("PrimaryCamera");
	camera->setNearClipDistance(2.0f);
	camera->setFarClipDistance(1000.f);
	Ogre::SceneNode *cameraNode = m_scene->getRootSceneNode()->createChildSceneNode("PrimaryCameraNode");
	cameraNode->attachObject(camera);
	m_cameras.push_back(Camera {camera, cameraNode});
	game.SetCamera(camera);
	cameraNode->setPosition(Ogre::Vector3(100, 100, 100));
	camera->lookAt(Ogre::Vector3(0, 0, 0));

	// create some cars
	for (int i = 0; i < 25; ++i) {
		m_objects.push_back(GameObject(*this, "CarBox", 30, 80, 20));
		m_objects.back().SetX((i % 5) * 10.f);
		m_objects.back().SetZ((i / 5) * 10.f);
	}

	// create some pedestrians
	for (int i = 0; i < 10; ++i) {
		m_objects.push_back(GameObject(*this, "PedBox", 180, 2, 2));
		m_objects.back().SetX(i * 5);
		m_objects.back().SetZ(45 - i * 5);
	}
}

bool GameWorld::Update()
{
	for (auto& object : m_objects) {
		object.Update();
	}

	// simulate physics

	for (auto& object : m_objects) {
		object.Render();
	}

	return true;
}

Ogre::SceneManager *GameWorld::GetScene()
{
	return m_scene;
}

Ogre::MeshPtr GameWorld::GetMesh(std::string meshName)
{
	return m_meshes[meshName];
}

Ogre::MeshPtr CreateBox(Ogre::SceneManager *scene, Ogre::String name, double w, double h, double d, Ogre::ColourValue color)
{
	Ogre::ManualObject *temp = scene->createManualObject(name);
	temp->setDynamic(false);

	temp->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	{
		temp->position(-w,  h, -d);
		temp->colour(color);
		temp->position( w,  h, -d);
		temp->colour(color);
		temp->position( w, -h, -d);
		temp->colour(color);
		temp->position(-w, -h, -d);
		temp->colour(color);

		temp->position(-w,  h,  d);
		temp->colour(color);
		temp->position( w,  h,  d);
		temp->colour(color);
		temp->position( w, -h,  d);
		temp->colour(color);
		temp->position(-w, -h,  d);
		temp->colour(color);

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
