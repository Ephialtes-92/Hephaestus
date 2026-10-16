#pragma once

#include <ThirdParty/GLFW/glfw3.h>
#include <string>

class Window
{
	int m_Height, m_Width = 1;
	std::string m_Title;
	GLFWwindow* m_Window = nullptr;
	//Key callbacks
	void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Window(int width, int height, std::string title);
	~Window();
	GLFWwindow* getWindow() { return m_Window; }
	void processInput();
	void onUpdate();
	bool shouldClose();
};