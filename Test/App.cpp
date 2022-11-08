#include "App.h"
#include "Box.h"
#include "Melon.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "SkinnedBox.h"
#include <memory>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include "Math.h"
#include "AssImpTest.h"

App::App() : wnd(800, 600, "Test App"), light(wnd.Gfx()), skybox(wnd.Gfx())
{

	class Factory
	{
	public:
		Factory(Graphics& gfx) : gfx(gfx) {}

		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };

			switch (sdist(rng))
			{			
			case 0:
				return std::make_unique<Box>(
					gfx,rng,adist,ddist,
					odist,rdist,bdist,mat
				);
			case 1:
				return std::make_unique<Cylinder>(
					gfx,rng,adist,ddist,odist,
					rdist,bdist,tdist
				);
			case 2:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist, odist,
					rdist, tdist
					);
			case 3:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			default:
				assert(false && "impossible drawable option in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0,3 };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_int_distribution<int> tdist{ 3,30 };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

App::~App()
{}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;

	wnd.Gfx().BeginFrame(0.1f, 0.2f, 0.2f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());
	wnd.Gfx().SetStencile(true);
	skybox.Draw(wnd.Gfx());
	wnd.Gfx().SetStencile(false);
	for (auto& d : drawables)
	{
		d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());

	static char buffer[1024];

	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("Aplication average %.3f ms/frame (%.1f FPS)", 1000.0f/ ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();

	cam.SpawnControlWindow();
	light.SpawnControlWindow();

	wnd.Gfx().EndFrame();
}

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