#pragma once
#include "stdafx.h"
#include "Ball.h"
#include <glm.hpp>
#include "Shader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc\type_ptr.hpp> 
#include "Player.h"
#include "Time.h"

Ball::Ball() {
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Ball::move() {
	this->pos.x += (directionX * (speed * Time::deltaTime));
	this->pos.y += (directionY * (speed * Time::deltaTime));
}
void Ball::collide(glm::vec3 p1Pos, glm::vec3 p2Pos) {
	float sp = (speed * Time::deltaTime);
	//float padWidth = 0.0f;
	//Collide on top
	if ((this->pos.y + sp) <= -0.8f || (this->pos.y + sp) >= 0.8f) {
		this->directionY = directionY * (-1);
	}
	//Collide with players
	if ((this->pos.x + sp) <= (p1Pos.x + Player::padWidth) ) {
		if (p1Pos.y >= this->pos.y && (p1Pos.y - 0.5f) <= this->pos.y) {
			if ((this->pos.x + sp) > p1Pos.x ) {
				this->directionX = directionX * (-1);
			}
			return;
		}
	}
	if ((this->pos.x + sp) >= p2Pos.x) {
		if (p2Pos.y >= this->pos.y && (p2Pos.y - 0.5f) <= this->pos.y) {
			if ((this->pos.x + sp) < (p2Pos.x + Player::padWidth)) {
				this->directionX = directionX * (-1);
			}
			return;
		}
	}
	//Collide on X (Resets)
	if ((this->pos.x + sp) <= -1.0f || (this->pos.x + sp) >= 1.0f) {
		this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
		this->directionX =0.0f;
		this->directionY = 0.0f;
	}
}
void Ball::render(Shader ourS, unsigned int EBO, glm::vec3 p1Pos, glm::vec3 p2Pos) {
	collide(p1Pos, p2Pos);
	move();
	glm::mat4 model;
	model = glm::translate(model, this->pos);
	GLint transformLocation = glGetUniformLocation(ourS.Program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}