// PingPong.cpp: define el punto de entrada de la aplicación de consola.
//
#include "stdafx.h"
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#define GLEW_STATIC 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL2\SOIL2.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc\type_ptr.hpp> 
#include "Shader.h"
#include <thread>
#include <WinSock2.h>
#include <string>

//Window Dimensions
const GLint WIDTH = 900, HEIGHT = 680;
int padHeight = 10;

const GLchar *vertexShaderSource;

const  GLchar *fragmentShaderSource;

GLFWwindow *window;

SOCKET connections[2];
int connectionCounter = 0;
int x = 0;
int prevx = 0;
std::string ip;

void solveValue(int val) {
	switch (val) {
	case 1:
		x += padHeight;
		break;
	case 0:
		x -= padHeight;
		break;
	}
}

void clientHandlerThread(int index) {
	char buffer[1];
	while (true) {
		recv(connections[index], buffer, 1, NULL);
		int z = buffer[0];
		std::cout << z << std::endl;
		solveValue(z);
	}
}



void handleNetwork() {
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr; //Address that will bind our listening socket to
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());	//Broadcast locally
	addr.sin_port = htons(6590);	//Port
	addr.sin_family = AF_INET; //IPV4 socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the address to the socket
	listen(sListen, SOMAXCONN);	//pLACES sListen socket in a state in which is listening for an incoming connection, SOMAXCONN = Socket Outstanding

	SOCKET newConnection;		//Socket to hold the client's connection

	for (int i = 0; i < 2; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0)		//If accepting the client's connection failed
		{
			std::cout << "Failed to accept the client's connection " << std::endl;
		}
		else {
			std::cout << "Client connected! " << std::endl;
			char MOTD[256] = "Hello";
			send(newConnection, MOTD, sizeof(MOTD), NULL); //send MOTD to buffer
			connections[i] = newConnection;
			connectionCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandlerThread, (LPVOID)(i), NULL, NULL); //Create thread
		}
	}
}

void input() {
	while (1) {
		if (prevx != x) {
			
			//glm::mat4 transform;
			//transform = glm::translate(transform, glm::vec3(0.1f, -0.5f, 0.0f));
			//prevx = x;
		}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		x += padHeight;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		x -= padHeight;
	}
}

int initWindow() {

	//Init GLFW
	glfwInit();

	//Sets all required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Create a GLFWWindow object that we can use for GLFW's functions
	window = glfwCreateWindow(WIDTH, HEIGHT, "Ping Pong", nullptr, nullptr);

	//Obtains size using for screen, ensures maximun compact, and pixel perfect look
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failet to init GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SOURCE0_ALPHA, GL_ONE_MINUS_SRC_COLOR);

	glfwSetKeyCallback(window, key_callback);

	return EXIT_SUCCESS;

	//----------------------------------------------------------------------------------------------//
}



int main() {

	std::cout << "Ingresa la direccion IP de esta máquina en la red." << std::endl;
	std::getline(std::cin, ip);

	std::thread first(input);     // spawn new thread that calls foo()

	if (initWindow() == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)handleNetwork, NULL, NULL, NULL); //Create thread

	//Mesh setup
	Shader ourShader("./core.vs", "./core.frag");

	//Triangle Vertices
	GLfloat vertices[] = {
		//Position				Colors				Texture Coordinates
		-0.99f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	//Bottom left
		-0.98f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	//Bottom Right
		-0.99f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,	//Top Left

		-0.98f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	
		-0.98f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,	
		-0.99f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,		
	};

	//Vertex Buffer Object, Vertex Array Object
	GLuint bufferId, vertexArrayId;

	//Generating buffer for vertex data
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	//Enabling Attribute 0 of vertex, corresponding to position
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLfloat prev = 1;

	//Game Loop
	while (!glfwWindowShouldClose(window)) {

		//Check if any events have been activated (key pressed, mouse moved, etc.) and call corresponding response functions
		glfwPollEvents();

		//Render
		//Clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		GLfloat FPS = 1 / ((GLfloat)glfwGetTime() - prev);
		prev = (GLfloat)glfwGetTime();

		//std::cout << glfwGetTime() * 0.1f << std::endl;

		//draw stuff
		ourShader.Use();

		//Translation using the x value
		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.0f,0.05f * x,0.9f));

		GLint transformLocation = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(vertexArrayId);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//Swap the screen buffers
		glfwSwapBuffers(window);		
	}

	glDeleteVertexArrays(1, &vertexArrayId);
	glDeleteBuffers(1, &bufferId);

	first.join();                // pauses until first finishes
	//Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();

	return EXIT_SUCCESS;

}



//#include <GL/freeglut.h>
//#include <time.h>
//#include <cmath>
//#include "Player.h"
//#include "Score.h"
//#include "Ball.h"





//
//class Game {
//public:
//
//	const int WINDOW_WIDTH = 1280;
//	const int WINDOW_HEIGHT = 720;
//	Player p1;
//	Player p2;
//	Score score;
//	int playerSpeed;
//	int ballSpeed;
//
//	void start_settings();
//	void win();
//	void DrawField();
//	void DrawScore(); 
//
//	
//} settings;
//
//void Game::start_settings() {
//
//}
//void Game::win() {
//
//}
//void Game::DrawField() {
//
//}
//void Game::DrawScore() {
//
//}
//
//void keyboard(unsigned char key, int x, int y) {
//	switch (key) {
//	case 'q':
//		cout << "Draw" << endl;
//		//left.Up = true;
//		break;
//	case 'a':
//		//left.Down = true;
//		break;
//	case 'z':
//		//if (left.hold) {
//		//	left.hold = false;
//			//ball.vx = settings.BallSpeedX;
//		//}
//		break;
//	case 'p':
//		//right.Up = true;
//		break;
//	case 'l':
//		//right.Down = true;
//		break;
//	case 'm':
//		/*if (right.hold) {
//			right.hold = false;
//			//ball.vx = -settings.BallSpeedX;
//			break;
//		}*/
//		break;
//	}
//}
//
//void keyboardUp(unsigned char key, int x, int y) {
//	switch (key) {
//	case 'q':
//		//left.Up = false;
//		break;
//	case 'a':
//		//left.Down = false;
//		break;
//	case 'p':
//		//right.Up = false;
//		break;
//	case 'l':
//		//right.Down = false;
//		break;
//	}
//}
//
//
//void Render() {
//	/*glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_QUADS);
//	settings.DrawField();
//	glEnd();
//	settings.DrawScore();
//	glutSwapBuffers();*/
//}
//
//int main(int argc, char ** argv) {
//	std::cout << "Hello" << std::endl;
//	std::cin.get();
//	/*glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE);
//	glutInitWindowSize(settings.WINDOW_WIDTH, settings.WINDOW_HEIGHT);
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow("Ping Pong");
//	glutDisplayFunc(Render);*/
//	//glutKeyboardFunc(keyboard);
//	//glutKeyboardUpFunc(keyboardUp);
//	/*glClearColor(0, 0, 0, 1.0);
//	glutMainLoop();*/
//	return 0;
//}