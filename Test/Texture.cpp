#include "Texture.h"
#include "GraphicsThrowMacros.h"
#include <WICTextureLoader.h>

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const std::wstring file)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(GetDevice(gfx), file.c_str(), &pTexture, &pTextureView, 0));
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
