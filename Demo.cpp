#include "Demo.h"


Demo::Demo()
{
}


Demo::~Demo()
{
}

void Demo::Init()
{

	BuildBackSprite();
	BuildScore2();
	BuildScore1();
	BuildPlayerSprite();
	for (int i = 0; i < foodQty; i++) {
		foodLeftToRight[i] = rand() % 2;
		BuildFoodSprite(i);
	}
	for (int i = 0; i < predatorQty; i++) {
		BuildEnemySprite(i);
	}
}

void Demo::Update(float deltaTime)
{
	if (gameTime <= 3000.0f) {
		gameTime += deltaTime;
	}

	if (IsKeyDown("Quit")) {
		cout << "Final Score: " << score << endl;
		SDL_Quit();
		exit(0);
	}

	UpdatePlayerSpriteAnim(deltaTime);
	ControlPlayerSprite(deltaTime);

	for (int i = 0; i < foodQty; i++) {
		FoodSpawn(i, deltaTime);
	}
	for (int i = 0; i < predatorQty; i++) {
		PredatorSpawn(i, deltaTime);
	}
}

void Demo::Render()
{
	//Setting Viewport
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	DrawBackSprite();
	DrawScore2();
	DrawScore1();
	DrawPlayerSprite();
	for (int i = 0; i < foodQty; i++) {
		DrawFoodSprite(i);
	}
	for (int i = 0; i < predatorQty; i++) {
		DrawEnemySprite(i);
	}	
}

void Demo::UpdatePlayerSpriteAnim(float deltaTime)
{
	// Update animation
	frame_dur += deltaTime;

	if (walk_anim && frame_dur > FRAME_DUR) {
		frame_dur = 0;
		if (frame_idx == NUM_FRAMES - 1) frame_idx = 0;  else frame_idx++;

		// Pass frameIndex to shader
		UseShader(this->program);
		glUniform1i(glGetUniformLocation(this->program, "frameIndex"), frame_idx);
		UseShader(this->program3);
		glUniform1i(glGetUniformLocation(this->program3, "frameIndex"), frame_idx);
		UseShader(this->program2);
		glUniform1i(glGetUniformLocation(this->program2, "frameIndex"), frame_idx);
	}

}

void Demo::ControlPlayerSprite(float deltaTime)
{
	walk_anim = true;
	player.oldxpos = player.xpos;
	player.oldypos = player.ypos;
	
	if (IsKeyDown("Move Right")) {
		player.xpos += deltaTime * player.velocity;
		flip = 0;
	}

	if (IsKeyDown("Move Left")) {
		player.xpos -= deltaTime * player.velocity;
		flip = 1;
	}

	if (IsKeyDown("Move Up")) {
		player.ypos -= deltaTime * player.velocity;
	}

	if (IsKeyDown("Move Down")) {
		player.ypos += deltaTime * player.velocity;
	}

	/*
	if (ypos2 > ypos){
		ypos += deltaTime * xVelocity;
	} else {
		ypos -= deltaTime * xVelocity;
	}

	if (xpos2 > xpos) {
		xpos += deltaTime * xVelocity;
		flip = 0;
	}
	else {
		xpos -= deltaTime * xVelocity;
		flip = 1;
	}
	*/

	if (player.ypos < 0 || player.ypos > GetScreenHeight() - frame_height || player.xpos < 0 || player.xpos > GetScreenWidth() - frame_width) {
		player.ypos = player.oldypos;
		player.xpos = player.oldxpos;
	}

}

void Demo::FoodSpawn(int index, float deltaTime) {
	if (foodLeftToRight[index] == 0) {
		if (foodPosX[index] > 0) {
			foodPosX[index] -= deltaTime * foodVelocity[index];
			int flipNo = 1;
			if (index == 0) {
				foodFlip0 = flipNo;
			} else if (index == 1) {
				foodFlip1 = flipNo;
			} else if (index == 2) {
				foodFlip2 = flipNo;
			} else if(index == 3) {
				foodFlip3 = flipNo;
			}
		}
		else {
			foodLeftToRight[index] = rand() % 2;
			if (foodLeftToRight[index] < 1) {
				foodPosX[index] = GetScreenWidth();
			}
			else {
				foodPosX[index] = 0;
			}
			foodPosY[index] = rand() % GetScreenHeight() - frame_height2;
			foodVelocity[index] = 0.3f + 0.005f * score - 0.01f * (rand() % 20);
		}
	}
	else if (foodLeftToRight[index] == 1) {
		if (foodPosX[index] < GetScreenWidth()) {
			foodPosX[index] += deltaTime * foodVelocity[index];
			int flipNo = 0;
			if (index == 0) {
				foodFlip0 = flipNo;
			}
			else if (index == 1) {
				foodFlip1 = flipNo;
			}
			else if (index == 2) {
				foodFlip2 = flipNo;
			}
			else if (index == 3) {
				foodFlip3 = flipNo;
			}
		}
		else {
			foodLeftToRight[index] = rand() % 2;
			if (foodLeftToRight[index] < 1) {
				foodPosX[index] = GetScreenWidth();
			}
			else {
				foodPosX[index] = 0;
			}
			foodPosY[index] = rand() % GetScreenHeight() - frame_height2;
			foodVelocity[index] = 0.3f + 0.005f * score - 0.01f * (rand() % 20);
		}
	}
	if (IsCollided(player.xpos, player.ypos, frame_width, frame_height, foodPosX[index], foodPosY[index], frame_width2, frame_height2)) {
		score++;
		BuildScore2();
		DrawScore2();
		BuildScore1();
		DrawScore1();
		cout << "Score: " << score << endl;
		foodLeftToRight[index] = rand() % 2;
		if (foodLeftToRight[index] < 1) {
			foodPosX[index] = GetScreenWidth();
		}
		else {
			foodPosX[index] = 0;
		}
		foodPosY[index] = rand() % GetScreenHeight() - frame_height2;
	}
}

void Demo::PredatorSpawn(int index, float deltaTime) {
	if (predatorLeftToRight[index] == 0) {
		if (predatorPosX[index] > 0) {
			predatorPosX[index] -= deltaTime * predatorVelocity[index];
			int flipNo = 1;
			if (index == 0) {
				predatorFlip0 = flipNo;
			} else if (index == 1) {
				predatorFlip1 = flipNo;
			} else if (index == 2) {
				predatorFlip2 = flipNo;
			} else if(index == 3) {
				predatorFlip3 = flipNo;
			}
		}
		else {
			predatorLeftToRight[index] = rand() % 2;
			if (predatorLeftToRight[index] < 1) {
				predatorPosX[index] = GetScreenWidth();
			}
			else {
				predatorPosX[index] = 0;
			}
			predatorPosY[index] = rand() % GetScreenHeight() - frame_height3;
			predatorVelocity[index] = 0.3f + 0.005f * score - 0.01f * (rand() % 20);
		}
	}
	else if (predatorLeftToRight[index] == 1) {
		if (predatorPosX[index] < GetScreenWidth()) {
			predatorPosX[index] += deltaTime * predatorVelocity[index];
			int flipNo = 0;
			if (index == 0) {
				predatorFlip0 = flipNo;
			}
			else if (index == 1) {
				predatorFlip1 = flipNo;
			}
			else if (index == 2) {
				predatorFlip2 = flipNo;
			}
			else if (index == 3) {
				predatorFlip3 = flipNo;
			}
		}
		else {
			predatorLeftToRight[index] = rand() % 2;
			if (predatorLeftToRight[index] < 1) {
				predatorPosX[index] = GetScreenWidth();
			}
			else {
				predatorPosX[index] = 0;
			}
			predatorPosY[index] = rand() % GetScreenHeight() - frame_height3;
			predatorVelocity[index] = 0.3f + 0.005f * score - 0.01f * (rand() % 20);
		}
	}
	if (IsCollided(player.xpos, player.ypos, frame_width, frame_height, predatorPosX[index], predatorPosY[index], frame_width3, frame_height3)) {
		if (gameTime <= 3000.0f) {
			predatorLeftToRight[index] = rand() % 2;
			if (predatorLeftToRight[index] < 1) {
				predatorPosX[index] = GetScreenWidth();
			}
			else {
				predatorPosX[index] = 0;
			}
			predatorPosY[index] = rand() % GetScreenHeight() - frame_height3;
		}
		else {
			cout << "Final Score: " << score << endl;
			SDL_Quit();
			exit(0);
		}
	}
}

void Demo::DrawPlayerSprite() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "ourTexture"), 0);

	// set flip
	glUniform1i(glGetUniformLocation(this->program, "flip"), flip);
	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(player.xpos, player.ypos, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width, frame_height, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

void Demo::BuildPlayerSprite()
{
	this->program = BuildShader("playerSprite.vert", "playerSprite.frag");

	// Pass n to shader
	UseShader(this->program);
	glUniform1f(glGetUniformLocation(this->program, "n"), 1.0f / NUM_FRAMES);

	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("playerclear45.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	// Set up vertex data (and buffer(s)) and attribute pointers
	frame_width = ((float)width) / NUM_FRAMES;
	frame_height = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program, "projection"), 1, GL_FALSE, value_ptr(projection));

	player.xpos = (GetScreenWidth() - frame_width) / 2;
	player.ypos = (GetScreenHeight() - frame_height) / 2;
	//player.velocity = 0.5f;

	// Add input mapping
	// to offer input change flexibility you can save the input mapping configuration in a configuration file (non-hard code) !
	InputMapping("Move Right", SDLK_RIGHT);
	InputMapping("Move Left", SDLK_LEFT);
	InputMapping("Move Up", SDLK_UP);
	InputMapping("Move Down", SDLK_DOWN);
	InputMapping("Move Right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	InputMapping("Move Left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	InputMapping("Quit", SDLK_ESCAPE);
}

void Demo::DrawEnemySprite(int index) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	UseShader(this->program3);
	glUniform1i(glGetUniformLocation(this->program3, "ourTexture"), 0);

	// set flip
	if (index == 0) {
		glUniform1i(glGetUniformLocation(this->program3, "flip"), predatorFlip0);
	} else if (index == 1) {
		glUniform1i(glGetUniformLocation(this->program3, "flip"), predatorFlip1);
	} else if (index == 2) {
		glUniform1i(glGetUniformLocation(this->program3, "flip"), predatorFlip2);
	} else if (index == 3) {
		glUniform1i(glGetUniformLocation(this->program3, "flip"), predatorFlip3);
	}
	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(predatorPosX [index], predatorPosY[index], 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width3, frame_height3, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO3);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

void Demo::BuildEnemySprite(int index)
{
	this->program3 = BuildShader("predatorSprite.vert", "predatorSprite.frag");

	// Pass n to shader
	UseShader(this->program3);
	glUniform1f(glGetUniformLocation(this->program3, "n"), 1.0f / NUM_FRAMES);

	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("enemyclear60.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	// Set up vertex data (and buffer(s)) and attribute pointers
	frame_width3 = ((float)width) / NUM_FRAMES;
	frame_height3 = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "projection"), 1, GL_FALSE, value_ptr(projection));

	// set sprite position, gravity, velocity
	predatorPosX[index] = (GetScreenWidth() - frame_width3) / 2;
	predatorPosY[index] = (GetScreenHeight() - frame_height3) / 2;
	//gravity = 0.05f;
	predatorVelocity[index] = 0.3f + 0.005f * score - 0.01f * (rand() % 20);
}

void Demo::BuildFoodSprite(int index)
{
	this->program2 = BuildShader("foodSprite.vert", "foodSprite.frag");

	// Pass n to shader
	UseShader(this->program2);
	glUniform1f(glGetUniformLocation(this->program2, "n"), 1.0f / NUM_FRAMES);

	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

											// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("foodclear30.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_width2 = ((float)width) / NUM_FRAMES;
	frame_height2 = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "projection"), 1, GL_FALSE, value_ptr(projection));

	foodPosX[index] = (GetScreenWidth() - frame_width2) / 4;
	foodPosY[index] = (GetScreenHeight() - frame_height2) / 4;
	foodVelocity[index] = 0.3f + 0.005f * score - 0.01f * (rand() % 20);
}

void Demo::DrawFoodSprite(int index) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	UseShader(this->program2);
	glUniform1i(glGetUniformLocation(this->program2, "ourTexture"), 0);

	// set flip
	if (index == 0) {
		glUniform1i(glGetUniformLocation(this->program2, "flip"), foodFlip0);
	}
	else if (index == 1) {
		glUniform1i(glGetUniformLocation(this->program2, "flip"), foodFlip1);
	}
	else if (index == 2) {
		glUniform1i(glGetUniformLocation(this->program2, "flip"), foodFlip2);
	}
	else if (index == 3) {
		glUniform1i(glGetUniformLocation(this->program2, "flip"), foodFlip3);
	}
	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(foodPosX[index], foodPosY[index], 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width2, frame_height2, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO2);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

bool Demo::IsCollided(float x1, float y1, float width1, float height1,
	float x2, float y2, float width2, float height2) {
	return (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2);
}

void Demo::BuildBackSprite()
{
	this->program1 = BuildShader("backgroundSprite.vert", "backgroundSprite.frag");
	UseShader(this->program1);

	// Load and create a texture 
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("Background.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	// Set up vertex data (and buffer(s)) and attribute pointers
	frame_width1 = (float)width;
	frame_height1 = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);

	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program1, "projection"), 1, GL_FALSE, value_ptr(projection));
}

void Demo::DrawBackSprite() {
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Activate shader
	UseShader(this->program1);
	glUniform1i(glGetUniformLocation(this->program1, "ourTexture"), 1);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(0, 0  - 0.4f * frame_height1, -0.2f));
	// Scale sprite 
	model = scale(model, vec3(frame_width1, frame_height1, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program1, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO1);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

void Demo::BuildScore2()
{
	this->sc2program = BuildShader("backgroundSprite.vert", "backgroundSprite.frag");
	UseShader(this->sc2program);

	// Load and create a texture 
	glGenTextures(1, &sc2texture);
	glBindTexture(GL_TEXTURE_2D, sc2texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

											// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	if (score / 10 == 0) {
		unsigned char* image = SOIL_load_image("0.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 1) {
		unsigned char* image = SOIL_load_image("1.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 2) {
		unsigned char* image = SOIL_load_image("2.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 3) {
		unsigned char* image = SOIL_load_image("3.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 4) {
		unsigned char* image = SOIL_load_image("4.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 5) {
		unsigned char* image = SOIL_load_image("5.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 6) {
		unsigned char* image = SOIL_load_image("6.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 7) {
		unsigned char* image = SOIL_load_image("7.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 8) {
		unsigned char* image = SOIL_load_image("8.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score / 10 == 9) {
		unsigned char* image = SOIL_load_image("9.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	sc2frame_width = (float)width;
	sc2frame_height = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &sc2VAO);
	glGenBuffers(1, &sc2VBO);
	glGenBuffers(1, &sc2EBO);

	glBindVertexArray(sc2VAO);

	glBindBuffer(GL_ARRAY_BUFFER, sc2VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sc2EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->sc2program, "projection"), 1, GL_FALSE, value_ptr(projection));
}

void Demo::DrawScore2() {
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sc2texture);
	// Activate shader
	UseShader(this->sc2program);
	glUniform1i(glGetUniformLocation(this->sc2program, "ourTexture"), 1);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(0, 0, -0.2f));
	// Scale sprite 
	model = scale(model, vec3(sc2frame_width, sc2frame_height, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->sc2program, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(sc2VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

void Demo::BuildScore1()
{
	this->sc1program = BuildShader("backgroundSprite.vert", "backgroundSprite.frag");
	UseShader(this->sc1program);

	// Load and create a texture 
	glGenTextures(1, &sc1texture);
	glBindTexture(GL_TEXTURE_2D, sc1texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

											// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	if (score % 10 == 0) {
		unsigned char* image = SOIL_load_image("0.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 1) {
		unsigned char* image = SOIL_load_image("1.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 2) {
		unsigned char* image = SOIL_load_image("2.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 3) {
		unsigned char* image = SOIL_load_image("3.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 4) {
		unsigned char* image = SOIL_load_image("4.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 5) {
		unsigned char* image = SOIL_load_image("5.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 6) {
		unsigned char* image = SOIL_load_image("6.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 7) {
		unsigned char* image = SOIL_load_image("7.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 8) {
		unsigned char* image = SOIL_load_image("8.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (score % 10 == 9) {
		unsigned char* image = SOIL_load_image("9.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// Set up vertex data (and buffer(s)) and attribute pointers
	sc1frame_width = (float)width;
	sc1frame_height = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &sc1VAO);
	glGenBuffers(1, &sc1VBO);
	glGenBuffers(1, &sc1EBO);

	glBindVertexArray(sc1VAO);

	glBindBuffer(GL_ARRAY_BUFFER, sc1VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sc1EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->sc1program, "projection"), 1, GL_FALSE, value_ptr(projection));
}

void Demo::DrawScore1() {
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sc1texture);
	// Activate shader
	UseShader(this->sc1program);
	glUniform1i(glGetUniformLocation(this->sc1program, "ourTexture"), 1);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(sc1frame_width, 0, -0.2f));
	// Scale sprite 
	model = scale(model, vec3(sc1frame_width, sc1frame_height, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->sc1program, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(sc1VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

int main(int argc, char** argv) {

	Engine::Game &game = Demo();
	game.Start("Game", 900, 600, false, WindowFlag::WINDOWED, 60, 1);

	return 0;
}
