#pragma once
#include "DrawableBase.h"
#include "ConstantBuffers.h"
#include "Timer.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float dt) noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void Bond(Graphics& gfx);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	struct PSColorConstant
	{
		alignas(16)DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
		float time;
	};
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	Timer timer;
	mutable Bind::PixelConstantBuffer<PSColorConstant> cbuf;
};

