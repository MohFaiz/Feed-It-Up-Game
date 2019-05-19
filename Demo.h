#ifndef DEMO_H
#define DEMO_H


#include <SOIL/SOIL.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Game.h"
#include "GameObject.h"

#define NUM_FRAMES 8
#define FRAME_DUR 80

using namespace glm;

class Demo :
	public Engine::Game
{
public:
	Demo();
	~Demo();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	float frame_width = 0, frame_height = 0, frame_width2 = 0, frame_height2 = 0, frame_width3 = 0, frame_height3 = 0, frame_width1 = 0, frame_height1 = 0;
	float sc1frame_width = 0, sc1frame_height = 0, sc2frame_width = 0, sc2frame_height = 0;
private:
	GameObject player = GameObject(0.0f, 0.0f, 0.5f);
	int foodQty = 4;
	int predatorQty = 4;
	float* foodPosX = new float[foodQty];
	float* foodPosY = new float[foodQty];
	float* foodVelocity = new float[foodQty];
	int* foodLeftToRight = new int[foodQty];
	int* foodFlip = new int[foodQty];
	float* predatorPosX = new float[predatorQty];
	float* predatorPosY = new float[predatorQty];
	float* predatorVelocity = new float[predatorQty];
	int* predatorLeftToRight = new int[predatorQty];
	int score = 0;
	float frame_dur = 0, gameTime = 0;
	GLuint VBO2, VAO2, EBO2, texture2, program2, VBO3, VAO3, EBO3, texture3, program3, VBO1, VAO1, EBO1, texture1, program1;
	GLuint sc1VBO, sc1VAO, sc1EBO, sc1texture, sc1program, sc2VBO, sc2VAO, sc2EBO, sc2texture, sc2program;
	GLuint VBO, VAO, EBO, texture, program;
	bool walk_anim = false;
	unsigned int frame_idx = 0, flip = 0;
	unsigned int predatorFlip0 = 0, predatorFlip1 = 0, predatorFlip2 = 0, predatorFlip3 = 0;
	unsigned int foodFlip0 = 0, foodFlip1 = 0, foodFlip2 = 0, foodFlip3 = 0;
	void BuildPlayerSprite();
	void DrawPlayerSprite();
	void BuildEnemySprite(int index);
	void DrawEnemySprite(int index);
	void BuildFoodSprite(int index);
	void DrawFoodSprite(int index);
	void FoodSpawn(int index, float deltaTime);
	void BuildBackSprite();
	void DrawBackSprite();
	void DrawScore1();
	void BuildScore1();
	void DrawScore2();
	void BuildScore2();
	void PredatorSpawn(int index, float deltaTime);
	bool IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
	void UpdatePlayerSpriteAnim(float deltaTime);
	void ControlPlayerSprite(float deltaTime);
};
#endif

