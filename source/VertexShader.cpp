#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include <SCompiler.h>

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(CompileShaderFromFile(path.c_str(), "main", "vs_4_0", &pBytecodeBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader));
	}

	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}
}