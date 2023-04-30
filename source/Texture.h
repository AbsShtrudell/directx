#pragma once
#include "Bindable.h"

namespace Bind
{
	class Texture : public Bindable
	{

	public:
		Texture(Graphics& gfx, const std::wstring file);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
		Microsoft::WRL::ComPtr<ID3D11Resource> pTexture;
	};
}
