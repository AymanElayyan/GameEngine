#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"DWWWWWWWWWWWWWWWWWWWWWWD"
"DWWWWWWWWWWWWWWWWWWWWWWD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDDDDDDDDDDDDDDDDDDDDDDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DDWWWWWWWWWWWWWWWWWWWWDD"
"DWWWWWWWWWWWWWWWWWWWWWWD"
"DWWWWWWWWWWWWWWWWWWWWWWD";



Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
#if 1

	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/texture/RPGpack_sheet_2X.png");

	Hazel::FramebufferSpecification spec;
	spec.Width = 2000;
	spec.Height = 1000;
	m_Framebuffer = Hazel::Framebuffer::Create(spec);

	m_TextureStairs = Hazel::SubTexture2D::CreateFromCoord(m_SpriteSheet, { 7, 6 }, { 128, 128 });
	m_TextureBarrel = Hazel::SubTexture2D::CreateFromCoord(m_SpriteSheet, { 8, 2 }, { 128, 128 });
	m_TextureTree = Hazel::SubTexture2D::CreateFromCoord(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1,2 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	s_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCoord(m_SpriteSheet, { 6, 11 }, { 128, 128 });
	s_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCoord(m_SpriteSheet, { 11, 11 }, { 128, 128 });

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 3.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(7.0f);
#endif

}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Hazel::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;
#if 0
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}
#endif

#if 0
	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		//HZ_WARN("Hazel::Input::IsMouseButtonPressed");
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}
#endif

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Hazel::Ref<Hazel::SubTexture2D> texture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
				texture = s_TextureMap[tileType];

			Hazel::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);

		}
	}
	Hazel::Renderer2D::EndScene();
	}
}

void ShowExampleAppDockSpace(bool* p_open)
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{

			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}


void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	bool show = true;
	bool dockingEnable = true;

	ImGui::ShowDemoWindow(&show);
	ShowExampleAppDockSpace(&dockingEnable);

	ImGui::Begin("Settings");
	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
