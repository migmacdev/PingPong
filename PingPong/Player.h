#pragma once
#include <glm.hpp>
#include "Shader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc\type_ptr.hpp> 
class Player {
public:
	Player();
	Player(int ID);
	void move(float val);
	void render(Shader ourS, unsigned int EBO);
private:
	int id;
	glm::vec3 pos;
};