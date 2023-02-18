#pragma once

#include <vector>
#include <type_traits>
#include "Graphics.h"
#include "VertexLayout.h"
#include "ConditionalNoexcept.h"

namespace Dvtx
{
	class Vertex
	{
		friend class VertexBuffer;

	public:

		template<VertexLayout::ElementType Type>
		auto& Attr() noexcept(!IS_DEBUG)
		{
			auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG)
		{
			const auto& element = layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();

			switch (element.GetType())
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::BGRAColor:
				SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}
	protected:
		Vertex(char* pData, const VertexLayout& layout) noxnd;

	private:
		// enables parameter pack setting of multiple parameters by element index
		template<typename First, typename ...Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept(!IS_DEBUG)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		// helper to reduce code duplication in SetAttributeByIndex
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val) noexcept(!IS_DEBUG)
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				assert("Parameter attribute type mismatch" && false);
			}
		}

	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};

	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) noxnd;

		template<VertexLayout::ElementType Type>
		const auto& Attr() const noexcept(!IS_DEBUG)
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}

	private:
		Vertex vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout) noxnd;

		const char* GetData() const noxnd;
		const VertexLayout& GetLayout() const noexcept;

		size_t Size() const noxnd;
		size_t SizeBytes() const noxnd;
		template<typename ...Params>
		void EmplaceBack(Params&&... params) noexcept(!IS_DEBUG)
		{
			assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		Vertex Back() noxnd;
		Vertex Front() noxnd;
		Vertex operator[](size_t i) noxnd;
		ConstVertex Back() const noxnd;
		ConstVertex Front() const noxnd;
		ConstVertex operator[](size_t i) const noxnd;

	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
}