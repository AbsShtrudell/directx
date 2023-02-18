#pragma once

#include <vector>
#include <type_traits>
#include "Graphics.h"
#include "Color.h"
#include "ConditionalNoexcept.h"

namespace Dvtx
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count,
		};

		template<ElementType> struct Map;

		template<> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position";
		};

		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
		};

		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
		};

		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
		};

		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
		};

		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
		};

		template<> struct Map<BGRAColor>
		{
			using SysType = ::BGRAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset);
			size_t GetOffsetAfter() const noxnd;
			size_t GetOffset() const;
			size_t Size() const noxnd;
			static constexpr size_t SizeOf(ElementType type) noxnd;
			ElementType GetType() const noexcept;
			D3D11_INPUT_ELEMENT_DESC GetDesc() const noxnd;

		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc( size_t offset ) noxnd
			{
				return { Map<type>::semantic,0,Map<type>::dxgiFormat,0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
			}
		private:
			ElementType type;
			size_t offset;
		};

		public:
			template<ElementType Type>
			const Element& Resolve() const noxnd
			{
				for (auto& e : elements)
				{
					if (e.GetType() == Type)
					{
						return e;
					}
				}
				assert("Could not resolve element type" && false);
				return elements.front();
			}

			const Element& ResolveByIndex(size_t i) const noxnd;
			VertexLayout& Append(ElementType type) noxnd;
			size_t Size() const noxnd;
			size_t GetElementCount() const noexcept;
			std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noxnd;

		private:
			std::vector<Element> elements;
	};
}