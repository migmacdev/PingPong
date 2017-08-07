// PingPong.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <GL/freeglut.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include "Player.h"
#include "Score.h"
#include "Ball.h"

using namespace std;

class Game {
public:

	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 720;
	Player p1;
	Player p2;
	Score score;
	int playerSpeed;
	int ballSpeed;

	void start_settings();
	void win();
	void DrawField();
	void DrawScore(); 

	
} settings;

void Game::start_settings() {

}
void Game::win() {

}
void Game::DrawField() {

}
void Game::DrawScore() {

}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		cout << "Draw" << endl;
		//left.Up = true;
		break;
	case 'a':
		//left.Down = true;
		break;
	case 'z':
		//if (left.hold) {
		//	left.hold = false;
			//ball.vx = settings.BallSpeedX;
		//}
		break;
	case 'p':
		//right.Up = true;
		break;
	case 'l':
		//right.Down = true;
		break;
	case 'm':
		/*if (right.hold) {
			right.hold = false;
			//ball.vx = -settings.BallSpeedX;
			break;
		}*/
		break;
	}
}

void keyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		//left.Up = false;
		break;
	case 'a':
		//left.Down = false;
		break;
	case 'p':
		//right.Up = false;
		break;
	case 'l':
		//right.Down = false;
		break;
	}
}


void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	settings.DrawField();
	glEnd();
	settings.DrawScore();
	glutSwapBuffers();
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(settings.WINDOW_WIDTH, settings.WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ping Pong");
	glutDisplayFunc(Render);
	//glutKeyboardFunc(keyboard);
	//glutKeyboardUpFunc(keyboardUp);
	glClearColor(0, 0, 0, 1.0);
	glutMainLoop();
	return 0;
}