#include <hzpch.h>
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <Platform/OpenGL/OpenGLShader.h>

namespace Hazel
{
	struct Renderer2DStorge
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorge* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorge();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			//	  x      y     z    x color y	
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});

		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader> (s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader> (s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(
			{ position.x, position.y, 0.0f },
			size,
			color
		);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader> (s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader> (s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrowIndexed(s_Data->QuadVertexArray);
	}
}
