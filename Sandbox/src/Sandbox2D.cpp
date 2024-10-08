#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>
#include "Hazel/Core/MouseButtonCodes.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("SandBox2D"), m_CameraController(1288.0f / 720.0f, true), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/Texture/Checkerboard.png");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_TextureStaires = Hazel::SubTexture2D::CreateFromCoord(m_SpriteSheet, { 7, 6 }, { 128, 128 });

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 3.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Hazel::Renderer2D::ResetStats();

	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	Hazel::RenderCommand::Clear();

#if 0
	static float rotation = 0.0f;
	rotation += ts * 50.0f;

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 0.1f });
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f, { 0.0f, 0.0f, 0.0f, 0.0f });
	Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
	Hazel::Renderer2D::EndScene();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.1f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.1f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
			Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}
#endif

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		//HZ_WARN("Hazel::Input::IsMouseButtonPressed");
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPostion();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_SpriteSheet);
	Hazel::Renderer2D::EndScene();

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}


void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::Text("Renderer2D Stats:");
	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertices  : %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices   : %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));  // Allow color change
	ImGui::End();
}


void Sandbox2D::OnEvent(Hazel::Event& e)
{
	HZ_PROFILE_FUNCTION();
	m_CameraController.OnEvent(e);
}
