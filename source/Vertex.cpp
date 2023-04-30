#include "Vertex.h"

namespace Dvtx
{
	// Vertex
	Vertex::Vertex(char* pData, const VertexLayout& layout) noxnd
		:
	pData(pData),
		layout(layout)
	{
		assert(pData != nullptr);
	}

	ConstVertex::ConstVertex(const Vertex& v) noxnd
		:
	vertex(v)
	{}


	// VertexBuffer
	VertexBuffer::VertexBuffer(VertexLayout layout) noxnd
		:
	layout(std::move(layout))
	{}

	const char* VertexBuffer::GetData() const noxnd
	{
		return buffer.data();
	}

	const VertexLayout& VertexBuffer::GetLayout() const noexcept
	{
		return layout;
	}

	size_t VertexBuffer::Size() const noxnd
	{
		return buffer.size() / layout.Size();
	}

	size_t VertexBuffer::SizeBytes() const noxnd
	{
		return buffer.size();
	}

	Vertex VertexBuffer::Back() noxnd
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
	}

	Vertex VertexBuffer::Front() noxnd
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(),layout };
	}

	Vertex VertexBuffer::operator[](size_t i) noxnd
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i,layout };
	}

	ConstVertex VertexBuffer::Back() const noxnd
	{
		return const_cast<VertexBuffer*>(this)->Back();
	}

	ConstVertex VertexBuffer::Front() const noxnd
	{
		return const_cast<VertexBuffer*>(this)->Front();
	}

	ConstVertex VertexBuffer::operator[](size_t i) const noxnd
	{
		return const_cast<VertexBuffer&>(*this)[i];
	}
}