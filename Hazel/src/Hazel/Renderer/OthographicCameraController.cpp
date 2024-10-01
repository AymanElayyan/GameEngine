#include "hzpch.h"
#include "OthographicCameraControl.h"

#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"

namespace Hazel
{
	OthographicCameraControll::OthographicCameraControll(float aspecRatio, bool rotation)
		:m_Camera(-m_AspecRatio * m_ZoomLevel, m_AspecRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	
	void OthographicCameraControll::OnUpdate(Timestep timestep)
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
				m_RotationPosition += m_CameraRotationSpeed * timestep;
			else if (Input::IsKeyPressed(HZ_KEY_Q))
				m_RotationPosition -= m_CameraRotationSpeed * timestep;
			
			m_Camera.SetRotation(m_RotationPosition);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OthographicCameraControll::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OthographicCameraControll::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OthographicCameraControll::OnWindowResized));
	}

	bool OthographicCameraControll::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset();
		m_Camera.SetProjection(-m_AspecRatio * m_ZoomLevel, m_AspecRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OthographicCameraControll::OnWindowResized(WindowResizeEvent& e)
	{
		m_ZoomLevel = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspecRatio * m_ZoomLevel, m_AspecRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}