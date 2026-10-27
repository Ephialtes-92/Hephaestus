#pragma once

#include <memory>

#include <ThirdParty/glm/glm.hpp>

#include <hf/render/camera/Camera.h>
#include <hf/render/geometry/mesh.h>
#include <hf/render/shaders/shader.h>
#include <hf/render/textures/texture.h>
#include <hf/scene/IScene.h>

namespace hf::render
{
class Window;
}

namespace hf::scene
{
class CubeScene : public IScene
{
public:
    explicit CubeScene(render::Window& window);

    void Load() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Unload() override;

    void HandleKey(GLFWwindow* window, int key, int action) override;
    void HandleMouseMove(double xpos, double ypos) override;
    void HandleScroll(double xoffset, double yoffset) override;
    void HandleResize(int width, int height) override;

private:
    void ProcessMovementInput();

    render::Window& m_Window;
    bool m_WireframeMode = false;
    bool m_UiInteractionMode = false;
    bool m_ShowImGuiDemoWindow = false;
    float m_DeltaTime = 0.0f;
    float m_ResolutionX = 1920.0f;
    float m_ResolutionY = 1080.0f;

    render::Camera m_MainCamera;

    std::unique_ptr<render::Mesh> m_Cube;
    std::unique_ptr<render::Mesh> m_LightSource;
    std::unique_ptr<render::Texture> m_MetallicContainer;
    std::unique_ptr<render::Texture> m_MetallicContainerSpecular;
    std::unique_ptr<render::Shader> m_LightingShader;
    std::unique_ptr<render::Shader> m_LightSourceShader;

    glm::vec3 m_CubePositions[10] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 m_PointLightPositions[4] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    glm::vec3 m_LightColors[4] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.2f, 0.8f, 0.65f)
    };
};
}
