#pragma once
#include "Bindable.h"
class Rasterizer : public Bindable
{
public:
	Rasterizer(Graphics& gfx, bool twoSided);
	// Унаследовано через Bindable
	void Bind(Graphics& gfx) noexcept(!IS_DEBUG) override;
private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
};

