#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("SandBox2D"), m_CameraController(1288.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({-1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.3f, 0.5f, 0.7f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.8f }, { 0.4f, 0.1f, 0.5f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.9f, 0.2f, 0.7f, 1.0f });
	Hazel::Renderer2D::EndScene();
	//std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<Hazel::OpenGLShader> (m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Sequre Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
