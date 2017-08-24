#pragma once
#include <glm.hpp>
#include "Shader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc\type_ptr.hpp> 
class Ball {
public:
	Ball();
	void move();
	void render(Shader ourS, unsigned int EBO, glm::vec3 p1Pos, glm::vec3 p2Pos);
	void collide(glm::vec3, glm::vec3);
	float directionX = 0.0f;
	float directionY = 0.0f;
	float defaultDirX = 1.0f;
	float defaultDirY = 1.0f;
private:

	float speed = 0.4f;
	glm::vec3 pos;
};