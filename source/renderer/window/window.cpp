#include <renderer/window/window.h>
#include <iostream>

Window::Window(int width, int height, std::string title):
	m_Width(width),
	m_Height(height),
	m_Title(title)
{
	if (!glfwInit())
	{
		std::cout << "Please initialize GLFW before trying to create a window" << std::endl;
	}

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (m_Window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(m_Window);
}

Window::~Window()
{
	if (m_Window)
		glfwDestroyWindow(m_Window);

	glfwTerminate();

}

void Window::processInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, true);
}

void Window::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

