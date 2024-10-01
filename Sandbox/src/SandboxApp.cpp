#include <Hazel.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include <Hazel/EntryPoint.h>

#include <GLFW/include/GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: m_Camera(-2.0f, 2.0f, -1.4f, 1.4f), Layer("Example"),
		cameraPosition(0.0f), rotationPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			//    x      y     z             Color
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squareVertices[5 * 4] = {
		//	  x      y     z    x color y	
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		
		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset( Hazel::Shader::Create(vertexSrc, fragmentSrc));




		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";
		m_FlatColorShader.reset(Hazel::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));




		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;


			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";
		m_TextureShader.reset(Hazel::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));
	
		m_Texture = Hazel::Texture2D::Create("assets/Texture/Checkerboard.png");
		m_LogoTexture = Hazel::Texture2D::Create("assets/Texture/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Hazel::Timestep timestep) override
	{
		HZ_TRACE("Delta Time {0}, ({1}ms)", timestep.GetSeconds(), timestep.GetMilliseconds());

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT) || Hazel::Input::IsKeyPressed(HZ_KEY_A))
			cameraPosition.x += cameraSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT) || Hazel::Input::IsKeyPressed(HZ_KEY_D))
			cameraPosition.x -= cameraSpeed * timestep;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP) || Hazel::Input::IsKeyPressed(HZ_KEY_W))
			cameraPosition.y -= cameraSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN) || Hazel::Input::IsKeyPressed(HZ_KEY_S))
			cameraPosition.y += cameraSpeed * timestep;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
			rotationPosition += rotationSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
			rotationPosition -= rotationSpeed * timestep;

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(cameraPosition);
		m_Camera.SetRotation(rotationPosition);

		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 tramsform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, tramsform);
			}
		}

		m_Texture->Bind();
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVA,
			glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -0.25, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle 
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Sequre Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

	}

	void OnEvent(Hazel::Event& event) override
	{
	}

private:
	Hazel::Ref <Hazel::VertexArray> m_SquareVA;
	Hazel::Ref <Hazel::VertexArray> m_VertexArray;
	Hazel::Ref <Hazel::Shader> m_FlatColorShader ,m_TextureShader;
	Hazel::Ref <Hazel::Shader> m_Shader;

	Hazel::Ref <Hazel::Texture2D> m_Texture, m_LogoTexture;
	
	Hazel::OthographicCamera m_Camera;

	glm::vec3 cameraPosition;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
	glm::vec3 m_SquarePosition;

	float rotationPosition;
	float cameraSpeed = 3.0f;
	float rotationSpeed = 50.0f;
	float squareSpeed = 1.0f;

};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}