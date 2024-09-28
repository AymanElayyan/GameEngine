#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include <Hazel/Log.h>

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:		return new OpenGLVertexBuffer(vertices, size);

		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indeces, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:		return new OpenGLIndexBuffer(indeces, size);

		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}