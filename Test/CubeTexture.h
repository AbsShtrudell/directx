#pragma once
#include "Bindable.h"

class CubeTexture : public Bindable
{
public:
	CubeTexture(Graphics& gfx, const std::wstring& path, UINT slot = 0);
	void Bind(Graphics& gfx) noexcept(!IS_DEBUG) override;
private:
	unsigned int slot;
protected:
	std::wstring path;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};

