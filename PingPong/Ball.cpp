#pragma once
#include "stdafx.h"
#include "Ball.h"
#include <glm.hpp>
#include "Shader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc\type_ptr.hpp> 
#include "Player.h"

Ball::Ball() {
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Ball::move() {
	
	this->pos.x += (directionX * speed);
	this->pos.y += (directionY * speed);
}
void Ball::collide(glm::vec3 p1Pos, glm::vec3 p2Pos) {

	//Collide on top
	if ((this->pos.y + this->speed) < -0.8f || (this->pos.y + this->speed) > 0.8f) {
		this->directionY = directionY * (-1);
	}
	//Collide with players
	if ((this->pos.x + this->speed) < -0.99f) {
		std::cout <<  p1Pos.y << std::endl;
		if (p1Pos.y >= this->pos.y && (p1Pos.y - 0.5f) <= this->pos.y) {
			this->directionX = directionX * (-1);
			return;
		}
	}
	if ((this->pos.x + this->speed) > 0.99f) {
		std::cout << "P2 pos: " << p2Pos.y << std::endl;
		std::cout << "Ball pos: " << this->pos.y << std::endl;

		if (p2Pos.y >= this->pos.y && (p2Pos.y - 0.5f) <= this->pos.y) {
			this->directionX = directionX * (-1);
			return;
		}
	}


	//Collide on X (Resets)
	if ((this->pos.x + this->speed) < -1.0f || (this->pos.x + this->speed) > 1.0f) {
		this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
		//this->speed = 0.0f;
		this->directionX =0.0f;
		this->directionY = 0.0f;
		//this->directionX = directionX * (-1);
	}
	//if (o == 0)
	//{
	//	this->directionY = directionY * (-1);
	//}
	//if (o == 1 ) {
	//	this->directionX = directionX * (-1);
	//}
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