#pragma once
#include <Windows.h>
#include "GL\glew.h"
#include <GLFW/glfw3.h>
#include "MainScene.h"

class App final{
	friend int main();
	Scene* scene;
	static App* app;
	static App* App::GetApp();
	static const GLFWvidmode* mode;
	void Init();
	void Run();
	void Exit();
public:
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
};