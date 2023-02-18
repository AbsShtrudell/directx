#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Skybox.h"
#include "Mesh.h"
#include <set>

class App
{
public:
	App();
	~App();

	int Go();

private:
	void DoFrame();
	void ShowModelWindow();

private:
	ImguiManager imgui;
	Window wnd;
	Timer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	Skybox skybox;
	Model nano{ wnd.Gfx(),"..\\Resources\\nanosuit.obj" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};

