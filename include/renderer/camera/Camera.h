#pragma once

#include <ThirdParty/glm/glm.hpp>
#include <ThirdParty/glm/gtc/matrix_transform.hpp>

enum class CameraMovement
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT
};
class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 getViewMatrix();

	//Input Handling
	void move(CameraMovement direction, float deltaTime);
	void processMouseMovement(double xpos, double ypos);
	void handleZoom(double xoffset, double yoffset);

	//Setters/Getters
	float getFov() { return m_Fov; }
	void setFov(float fov)
	{
		m_Fov = fov;
		if (m_Fov < 1.0f)
			m_Fov = 1.0f;
		if (m_Fov > 45.0f)
			m_Fov = 45.0f;
	}

	float getSpeed() { return m_Speed; }
	void setSpeed(float speed) { m_Speed = speed; }

	float getZoomSpeed() { return m_ZoomSpeed; }
	void setZoomSpeed(float zoomSpeed) { m_ZoomSpeed = zoomSpeed; }

	float getSensitivity() { return m_Sensitivity; }
	void setSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }

	float getYaw() { return m_Yaw; }
	void setYaw(float yaw) { m_Yaw = yaw; }

	float getPitch() { return m_Pitch; }
	void setPitch(float pitch)
	{
		m_Pitch = pitch;
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	glm::vec3 getPosition() { return m_Position; }
	glm::vec3 getFront() { return m_Front; }
	glm::vec3 getUp() { return m_Up; }

private:
	//vectors
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_Target;
	glm::vec3 m_ViewMatrix;
	//Euler angles
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;
	//Settings
	float m_Speed = 6.0f;
	float m_ZoomSpeed = 2.0f;
	float m_Fov = 45.0f;
	float m_Sensitivity = 0.1f;
	//Needed to calculate the offset for movement
	//We can get these magic numbers from the window resolution???
	float m_lastX = 800.0f / 2.0;
	float m_lastY = 600.0 / 2.0;

	bool m_FirstMouse = false;
};