#include "hzpch.h"
#include "OthographicCameraController.h"

#include "Hazel/Core/Core.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel
{
	OthographicCameraController::OthographicCameraController(float aspecRatio, bool rotation)
		:m_AspecRatio(aspecRatio), m_Camera(-m_AspecRatio * m_ZoomLevel, m_AspecRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation),
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotation)
	{
	}

	
	void OthographicCameraController::OnUpdate(Timestep timestep)
	{
		HZ_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(HZ_KEY_LEFT) || Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(HZ_KEY_RIGHT) || Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(HZ_KEY_UP) || Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(HZ_KEY_DOWN) || Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * timestep;
			else if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * timestep;
			
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OthographicCameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OthographicCameraController::OnWindowResized));
	}

	bool OthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = (m_ZoomLevel) > (0.25f) ? (m_ZoomLevel) : (0.25f);
		m_Camera.SetProjection(-m_AspecRatio * m_ZoomLevel, m_AspecRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		return false;
	}

	bool OthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		m_AspecRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspecRatio * m_ZoomLevel, m_AspecRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}