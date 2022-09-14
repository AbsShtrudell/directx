#include "App.h"

#include <windows.h>
#include <sstream>
#include <iomanip>

App::App()
	: wnd(800, 600, "Test App")
{}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().Clearbuffer(0.0f, 0.5f, 1.0f);
	wnd.Gfx().DrawTestTriangle(
		timer.Peek(), 
		(wnd.mouse.GetPosX()) / 400.f - 1.f, 
		-(wnd.mouse.GetPosY()) / 300.f + 1.f
		);
	wnd.Gfx().DrawTestTriangle(-timer.Peek(),0,0);
	wnd.Gfx().EndFrame();
}
