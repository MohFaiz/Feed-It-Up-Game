#pragma once
class GameObject
{
public:
	GameObject();
	GameObject(float x, float y, float v);
	~GameObject();
	float xpos, ypos, velocity;
	float oldxpos = 0, oldypos = 0;
};

