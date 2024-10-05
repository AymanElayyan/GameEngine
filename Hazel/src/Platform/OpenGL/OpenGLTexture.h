#pragma once

#include "Hazel/Renderer/Texture.h"
#include <glad/glad.h>

namespace Hazel
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
		virtual void SetData(void* data, uint32_t size) override;

		virtual bool operator==(const Texture& other) const override
		{
			const OpenGLTexture2D* otherTexture = dynamic_cast<const OpenGLTexture2D*>(&other);
			if (!otherTexture)
				return false;

			return m_RendererID == otherTexture->m_RendererID;
		}

	
	private:
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;

		std::string m_Path;

		GLenum m_internalFormat, m_DataFormat;

	};
}
