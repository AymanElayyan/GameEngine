#pragma once

namespace Hazel
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		std::unique_ptr<GraphicsContext> Create(void* window);
	};
}
