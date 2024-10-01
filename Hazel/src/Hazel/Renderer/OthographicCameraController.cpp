#include "hzpch.h"
#include "OthographicCameraController.h"

#include "Hazel/Core.h"

#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"

namespace Hazel
{
	OthographicCameraController::OthographicCameraController(float aspecRatio, bool rotation)
		:m_AspecRatio(aspecRatio), m_Camera(-m_AspecRatio * m_ZoomLevel, m_AspecRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	
	void OthographicCameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(HZ_KEY_LEFT) || Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(HZ_KEY_RIGHT) || Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(HZ_KEY_UP) || Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(HZ_KEY_DOWN) || Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * timestep;
			else if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * timestep;
			
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OthographicCameraController::OnWindowResized));
	}

	bool OthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset();
		m_Camera.SetProjection(-m_AspecRatio * m_ZoomLevel, m_AspecRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspecRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspecRatio * m_ZoomLevel, m_AspecRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}