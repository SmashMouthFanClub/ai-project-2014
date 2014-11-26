#include "GameObject.h"

GameObject::GameObject(std::string meshName, double turn, double forward, double backward) :
	m_mesh(meshName), m_maxTurn(turn), m_maxForward(forward), m_maxBackward(backward)
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
}

void GameObject::Render()
{
}