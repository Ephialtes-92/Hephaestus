#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GLFW_INCLUDE_NONE
#include <ThirdParty/GLFW/glfw3.h>
#include <ThirdParty/glad/glad.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <renderer/window/window.h>
#include <memory>

#include <scenes/IScene.h>
#include <scenes/MainScene.h>
#include <scenes/SceneManager.h>

//Forward declarations
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float resolutionX = 1920.0f;
float resolutionY = 1080.0f;

IScene* activeScene = nullptr;


int main()
{
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);

    //Init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window(resolutionX, resolutionY, "OpenGL Renderer");

    //Key callbacks
    glfwSetKeyCallback(window.getWindow(), glfwKeyCallback);
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetScrollCallback(window.getWindow(), scroll_callback);

    //Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Define the viewport
    glViewport(0, 0, resolutionX, resolutionY);

    //Enable depth testing
    glEnable(GL_DEPTH_TEST);

    //Register GLFW callbacks
    glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    SceneManager sceneManager;
    auto mainScene = std::make_unique<MainScene>(window);
    activeScene = mainScene.get();
    sceneManager.ChangeScene(std::move(mainScene));

    float lastFrame = 0.0f;

    while (!window.shouldClose())
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sceneManager.Update(deltaTime);
        sceneManager.Render();
        window.onUpdate();
    }

    sceneManager.UnloadCurrent();
    activeScene = nullptr;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    if (activeScene)
        activeScene->HandleResize(width, height);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    if (activeScene)
        activeScene->HandleKey(window, key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;

    if (activeScene)
        activeScene->HandleMouseMove(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void)window;

    if (activeScene)
        activeScene->HandleScroll(xoffset, yoffset);
}