#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>

namespace Bind
{
	class Camera;

	class SkyboxTransformCbuf : public Bindable
	{
	protected:
		struct Transforms
		{
			DirectX::XMMATRIX viewProj;
		};
	public:
		SkyboxTransformCbuf(Graphics& gfx, UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noxnd;
		Transforms GetTransforms(Graphics& gfx) noxnd;
	private:
		std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	};
}
