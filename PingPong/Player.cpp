#pragma once
#include "stdafx.h"
#include "Player.h"
#include <glm.hpp>
#include "Shader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc\type_ptr.hpp> 

Player::Player() {
	this->id = -1;
}
Player::Player(int ID) {
	this->id = ID;
	if (ID == 0) {
		this->pos = glm::vec3(-0.99f, 0.0f, 0.0f);
	}
	else {
		this->pos = glm::vec3(0.99f, 0.0f, 0.0f);
	}
}
void Player::move(float val) {
	this->pos.y += val;
}
void Player::render(Shader ourS, unsigned int EBO) {
	if (this->id == -1) {
		return;
	}
	glm::mat4 model;
	model = glm::translate(model, this->pos);
	GLint transformLocation = glGetUniformLocation(ourS.Program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}