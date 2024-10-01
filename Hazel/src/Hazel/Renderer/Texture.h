#pragma once
#include <Hazel.h>

namespace Hazel
{
	class Texture
	{
	public:

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetWidth() const = 0;

		virtual void Bind() const = 0;
	};



	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		
	};
}
