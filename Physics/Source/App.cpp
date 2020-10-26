#include "App.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
using namespace std::chrono;

App* App::app = nullptr;
const GLFWvidmode* App::mode = nullptr;
float elapsedTime;
GLFWwindow* m_window;
int winWidth, winHeight;

App* App::GetApp(){
	if(!app){
		app = new App;
		app->Init();
	}
	return app;
}

static void error_callback(int, const char* description){
	fputs(description, stderr);
	_fgetchar();
}

bool App::IsKeyPressed(unsigned short key){
    return GetAsyncKeyState(key) & 0x8000;
}

bool App::IsMousePressed(unsigned short key){ //0 - Left, 1 - Right, 2 - Middle
	return glfwGetMouseButton(m_window, key) != 0;
}

void App::GetCursorPos(double* xpos, double* ypos){
	glfwGetCursorPos(m_window, xpos, ypos);
}

void App::Init(){
	system("color 0B");
	glfwSetErrorCallback(error_callback);
	if(!glfwInit()){
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	elapsedTime = 0.0;
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	winWidth = int(mode->width * 2 / 3);
	winHeight = winWidth * 3 / 4;
	m_window = glfwCreateWindow(winWidth, winHeight, "193541T Physics", NULL, NULL);

	if(!m_window){
		fprintf(stderr, "Failed to open GLFW win.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	} 
	glfwMakeContextCurrent(m_window);

	glewExperimental = 1; //Needed for core profile
	GLenum err = glewInit();
	if(err != GLEW_OK){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
}

void App::Run(){
	scene = new MainScene;
	glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
	glViewport(0, 0, mode->width, mode->height);
	dynamic_cast<MainScene*>(scene)->worldWidth = float(mode->width);
	dynamic_cast<MainScene*>(scene)->worldHeight = float(mode->height);
	scene->Init();
	auto timePt1 = system_clock::now();

	while(!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE)){
		system_clock::time_point timePt2 = system_clock::now();
		duration<float> frameDur = timePt2 - timePt1;
		timePt1 = timePt2;
		elapsedTime += frameDur.count();
		scene->Update(frameDur.count());
		scene->Render();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	Exit();
}

void App::Exit(){
	scene->Exit();
	delete scene;
	delete this;
	glfwDestroyWindow(m_window);
	glfwTerminate();
}