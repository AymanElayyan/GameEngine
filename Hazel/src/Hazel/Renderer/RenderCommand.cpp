#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRndererAPI.h"

namespace Hazel {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRndererAPI;

}