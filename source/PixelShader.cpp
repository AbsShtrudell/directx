#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include <SCompiler.h>

namespace Bind
{
	PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(CompileShaderFromFile(path.c_str(), "main", "ps_4_0", &pBytecodeBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pPixelShader));
	}

	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	ID3DBlob* PixelShader::GetBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}
}