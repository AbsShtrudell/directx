#include "Rasterizer.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Rasterizer::Rasterizer(Graphics& gfx, bool twoSided)
	{
		INFOMAN(gfx);

		D3D11_RASTERIZER_DESC rd = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rd.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rd, &pRasterizer));
	}

	void Rasterizer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->RSSetState(pRasterizer.Get());
	}
}