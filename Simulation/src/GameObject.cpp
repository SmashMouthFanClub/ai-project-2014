#include "GameObject.h"
#include "GameState.h"
#include "GameWorld.h"

GameObject::GameObject(GameWorld& gw, std::string meshName, double turn, double forward, double backward) :
	m_mesh(meshName), m_maxTurn(turn), m_maxForward(forward), m_maxBackward(backward), m_x(0), m_y(0), m_z(0)
{

}

void GameObject::SetX(double x)
{
	m_x = x;
}

void GameObject::SetY(double y)
{
	m_y = y;
}

void GameObject::SetZ(double z)
{
	m_z = z;
}

void GameObject::Update()
{
	// give AI agent 
	Action a = {0.2, 0.5};


}

void GameObject::Render()
{
}