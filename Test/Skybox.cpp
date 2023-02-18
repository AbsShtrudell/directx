#include "Skybox.h"
#include "Cube.h"
#include "CubeTexture.h"
#include "Sampler.h"
#include "GraphicsThrowMacros.h"
#include "BindableCommon.h"
#include "SkyboxTransformCbuf.h"
#include "Rasterizer.h"

using namespace Bind;

Skybox::Skybox(Graphics& gfx)
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT2 tex;
		};

		auto model = Cube::Make<Vertex>();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		AddStaticBind(std::make_unique<CubeTexture>(gfx, L"..\\Resources\\skymap.dds"));

		AddStaticBind(std::make_unique<Sampler>(gfx));

		AddStaticBind(std::make_unique<Rasterizer>(gfx,true));

		auto pvs = std::make_unique<VertexShader>(gfx, L"SkyboxVS.hlsl");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"SkyboxPS.hlsl"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	
		AddStaticBind(std::make_unique<SkyboxTransformCbuf>(gfx, 0u));
	}
	else
	{
		SetIndexFromStatic();
	}
}

void Skybox::Update(float dt) noexcept
{
}

DirectX::XMMATRIX Skybox::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixScaling(1, 1, 1);
}
