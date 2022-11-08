#include "Pyramid.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cone.h"
#include <array>

Pyramid::Pyramid(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<int>& tdist)
	:
	Object(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
			std::array<char, 4> color;
			char padding;
		};
		const auto tesselation = tdist(rng);
		auto model = Cone::MakeTesselatedIndependentFaces<Vertex>(tesselation);
		// set vertex colors for mesh
		for (auto& v : model.vertices)
		{
			v.color = { (char)10,(char)10,(char)255 };
		}
		for (int i = 0; i < tesselation; i++)
		{
			model.vertices[i * 3].color = { (char)255,(char)10,(char)10 };
		}

		model.vertices.front().color = { (char)255,(char)20,(char)20 }; // very first vertex is the cone tip
		// squash mesh a bit in the z direction
		model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));
		// add normals
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"BlendedPhongVS.hlsl");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"BlendedPhongPS.hlsl"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}