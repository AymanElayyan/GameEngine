#include "hzpch.h"
#include "SubTexture2D.h"

namespace Hazel
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoord[0] = { min.x, min.y };
		m_TexCoord[1] = { max.x, min.y };
		m_TexCoord[2] = { max.x, max.y };
		m_TexCoord[3] = { min.x, max.y };
	}
}