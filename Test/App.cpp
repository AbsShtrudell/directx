#include "App.h"

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
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
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}
