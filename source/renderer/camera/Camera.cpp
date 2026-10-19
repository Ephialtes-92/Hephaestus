#include <renderer/camera/Camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
	:m_Position(position),
	m_Front(front),
	m_Up(up),
    m_Right(glm::normalize(glm::cross(m_Front, m_Up))),
	m_Target(m_Position + m_Front)
{
}

glm::mat4 Camera::getViewMatrix()
{
    m_Target = m_Position + m_Front;
    return glm::lookAt(m_Position, m_Target, m_Up);
}

void Camera::move(CameraMovement direction, float deltaTime)
{
    float velocity = m_Speed * deltaTime;
    switch (direction)
    {
        case CameraMovement::FORWARD:
            m_Position += m_Front * velocity;
            break;
        case CameraMovement::BACKWARD:
            m_Position -= m_Front * velocity;
            break;
        case CameraMovement::LEFT:
            m_Position -= m_Right * velocity;
            break;
        case CameraMovement::RIGHT:
            m_Position += m_Right * velocity;
    }
}

void Camera::processMouseMovement(double xpos, double ypos)
{
    if (m_FirstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_FirstMouse = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos;
    m_lastX = xpos;
    m_lastY = ypos;

    xoffset *= m_Sensitivity;
    yoffset *= m_Sensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    //Clamp the pitch value
    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    direction.y = sin(glm::radians(m_Pitch));
    direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(direction);
	m_Right = glm::normalize(glm::cross(m_Front, m_Up));
}

void Camera::handleZoom(double xoffset, double yoffset)
{
    m_Fov -= (float)yoffset * m_ZoomSpeed;
    if (m_Fov < 1.0f)
        m_Fov = 1.0f;
    if (m_Fov > 45.0f)
        m_Fov = 45.0f;

}
