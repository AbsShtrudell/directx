#include "CubeTexture.h"
#include "GraphicsThrowMacros.h"
#include <DDSTextureLoader.h>

namespace wrl = Microsoft::WRL;

CubeTexture::CubeTexture(Graphics& gfx, const std::wstring& path, UINT slot)
: path(path), slot(slot)
{
	INFOMAN(gfx);
	wrl::ComPtr<ID3D11Resource> pTexture;

	GFX_THROW_INFO(DirectX::CreateDDSTextureFromFile(GetDevice(gfx), path.c_str(), &pTexture, &pTextureView, 0, nullptr));

	D3D11_SHADER_RESOURCE_VIEW_DESC SMTextureDesc;
	pTextureView.Get()->GetDesc(&SMTextureDesc);
	SMTextureDesc;
}

void CubeTexture::Bind(Graphics& gfx) noexcept(!IS_DEBUG)
{
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf()));
}