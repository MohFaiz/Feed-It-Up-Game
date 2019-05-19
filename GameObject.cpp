#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::GameObject(float x, float y, float v)
{
	xpos = x;
	ypos = y;
	velocity = v;
}

GameObject::~GameObject()
{
}
