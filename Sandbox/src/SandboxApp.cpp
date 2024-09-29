#include <Hazel.h>

#include <Hazel/EntryPoint.h>

#include <GLFW/include/GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), Layer("Example"),
		cameraPosition(0.0f), rotationPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			//    x      y     z             Color
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" }
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<Hazel::IndexBuffer> squareIB;
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

		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
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
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
		m_BlueShader.reset(new Hazel::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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


		if (Hazel::Input::IsKeyPressed(HZ_KEY_J) || Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_SquarePosition.x += squareSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L) || Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_SquarePosition.x -= squareSpeed * timestep;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_I) || Hazel::Input::IsKeyPressed(HZ_KEY_W))
			m_SquarePosition.y -= squareSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_K) || Hazel::Input::IsKeyPressed(HZ_KEY_S))
			m_SquarePosition.y += squareSpeed * timestep;
	

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(cameraPosition);
		m_Camera.SetRotation(rotationPosition);

		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 tramsform = glm::translate(glm::mat4(1.0f), m_SquarePosition);


		Hazel::Renderer::Submit(m_BlueShader, m_SquareVA, tramsform);
		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
	}

	void OnEvent(Hazel::Event& event) override
	{
	}

private:
	std::shared_ptr <Hazel::VertexArray> m_SquareVA;
	std::shared_ptr <Hazel::VertexArray> m_VertexArray;

	std::shared_ptr <Hazel::Shader> m_BlueShader;
	std::shared_ptr <Hazel::Shader> m_Shader;

	Hazel::OthographicCamera m_Camera;

	glm::vec3 cameraPosition;

	float rotationPosition;
	float cameraSpeed = 3.0f;
	float rotationSpeed = 50.0f;
	float squareSpeed = 1.0f;

	glm::vec3 m_SquarePosition;
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