#include "AssImpTest.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Texture.h"
#include "Sampler.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssImpTest::AssImpTest(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	float scale)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			struct
			{
				float u;
				float v;
			} tex;
		};

		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("..\\resources\\chest.obj",
			aiProcess_Triangulate
		);
		const auto pMesh = pModel->mMeshes[0];
		std::vector<Vertex> vertices;
		vertices.reserve(pMesh->mNumVertices);

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back({
				{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale },
				{  pMesh->mTextureCoords[0][i].x, 1- pMesh->mTextureCoords[0][i].y }});
		}

		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		AddStaticBind(std::make_unique<Texture>(gfx, L"..\\Resources\\chest_texture.png"));
		AddStaticBind(std::make_unique<Sampler>(gfx));
		auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.hlsl");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturePS.hlsl"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void AssImpTest::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX AssImpTest::GetTransformXM() const noexcept 
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}