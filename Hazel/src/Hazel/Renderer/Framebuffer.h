#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual const FramebufferSpecification& GetSpicification() const = 0;
		
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}

