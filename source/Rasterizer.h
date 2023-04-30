#pragma once
#include "Bindable.h"

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(Graphics& gfx, bool twoSided);
		// ������������ ����� Bindable
		void Bind(Graphics& gfx) noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
	};
}
