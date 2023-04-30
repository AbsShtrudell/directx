#pragma once
#include "DrawableBase.h"
#include "Graphics.h"

class Skybox : public DrawableBase<Skybox> 
{
public:
	Skybox(Graphics& gfx);
	void Update(float dt) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
};

