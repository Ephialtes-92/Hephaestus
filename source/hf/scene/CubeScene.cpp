#include <ThirdParty/glad/glad.h>

#include <hf/scene/CubeScene.h>

#include <filesystem>

#include <config.h>

#include <ThirdParty/glm/gtc/matrix_transform.hpp>

#include <hf/render/window/window.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace
{
    std::filesystem::path shaderDirectory = SHADERS_DIR;
}

namespace hf::scene
{

CubeScene::CubeScene(render::Window& window):
    m_Window(window)
{
}

void CubeScene::Load()
{
    float vertices3d[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    m_Cube = std::make_unique<render::Mesh>(vertices3d, sizeof(vertices3d));
    m_LightSource = std::make_unique<render::Mesh>(vertices3d, sizeof(vertices3d));

    m_LightingShader = std::make_unique<render::Shader>(
        shaderDirectory / "vertexShaderforLight.vert",
        shaderDirectory / "fragmentShaderforLight.frag"
    );
    m_LightSourceShader = std::make_unique<render::Shader>(
        shaderDirectory / "vertexShaderforLightSource.vert",
        shaderDirectory / "fragmentShaderforLightSource.frag"
    );

    m_MetallicContainer = std::make_unique<render::Texture>("container2.png");
    m_MetallicContainerSpecular = std::make_unique<render::Texture>("container2_specular.png");

    m_MetallicContainer->bind(0);
    m_MetallicContainerSpecular->bind(1);

    m_LightingShader->use();
    m_LightingShader->setInt("material.diffuse", 0);
    m_LightingShader->setInt("material.specular", 1);
}

void CubeScene::Update(float deltaTime)
{
    m_DeltaTime = deltaTime;

    m_Window.processInput();
    const ImGuiIO& io = ImGui::GetIO();
    if (!m_UiInteractionMode && !io.WantCaptureKeyboard)
        ProcessMovementInput();
}

void CubeScene::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_LightingShader->use();
    m_LightingShader->setFloat("material.shininess", 32.0f);

    glm::vec3 diffuseColor[] = {
        m_LightColors[0] * glm::vec3(0.8f),
        m_LightColors[1] * glm::vec3(0.8f),
        m_LightColors[2] * glm::vec3(0.8f),
        m_LightColors[3] * glm::vec3(0.8f)
    };

    glm::vec3 ambientColor[] = {
        diffuseColor[0] * glm::vec3(0.0f),
        diffuseColor[1] * glm::vec3(0.0f),
        diffuseColor[2] * glm::vec3(0.0f),
        diffuseColor[3] * glm::vec3(0.0f)
    };

    m_LightingShader->setVec3("lightColor", glm::vec3(1.0f));

    m_LightingShader->setFloat3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
    m_LightingShader->setVec3("directionalLight.ambient", glm::vec3(0.0f));
    m_LightingShader->setVec3("directionalLight.diffuse", glm::vec3(1.0f));
    m_LightingShader->setVec3("directionalLight.specular", glm::vec3(1.0f));

    m_LightingShader->setVec3("pointLights[0].position", m_PointLightPositions[0]);
    m_LightingShader->setVec3("pointLights[0].ambient", ambientColor[0]);
    m_LightingShader->setVec3("pointLights[0].diffuse", diffuseColor[0]);
    m_LightingShader->setVec3("pointLights[0].specular", glm::vec3(1.0f));
    m_LightingShader->setFloat("pointLights[0].constant", 1.0f);
    m_LightingShader->setFloat("pointLights[0].linear", 0.09f);
    m_LightingShader->setFloat("pointLights[0].quadratic", 0.032f);

    m_LightingShader->setVec3("pointLights[1].position", m_PointLightPositions[1]);
    m_LightingShader->setVec3("pointLights[1].ambient", ambientColor[1]);
    m_LightingShader->setVec3("pointLights[1].diffuse", diffuseColor[1]);
    m_LightingShader->setVec3("pointLights[1].specular", glm::vec3(1.0f));
    m_LightingShader->setFloat("pointLights[1].constant", 1.0f);
    m_LightingShader->setFloat("pointLights[1].linear", 0.09f);
    m_LightingShader->setFloat("pointLights[1].quadratic", 0.032f);

    m_LightingShader->setVec3("pointLights[2].position", m_PointLightPositions[2]);
    m_LightingShader->setVec3("pointLights[2].ambient", ambientColor[2]);
    m_LightingShader->setVec3("pointLights[2].diffuse", diffuseColor[2]);
    m_LightingShader->setVec3("pointLights[2].specular", glm::vec3(1.0f));
    m_LightingShader->setFloat("pointLights[2].constant", 1.0f);
    m_LightingShader->setFloat("pointLights[2].linear", 0.09f);
    m_LightingShader->setFloat("pointLights[2].quadratic", 0.032f);

    m_LightingShader->setVec3("pointLights[3].position", m_PointLightPositions[3]);
    m_LightingShader->setVec3("pointLights[3].ambient", ambientColor[3]);
    m_LightingShader->setVec3("pointLights[3].diffuse", diffuseColor[3]);
    m_LightingShader->setVec3("pointLights[3].specular", glm::vec3(1.0f));
    m_LightingShader->setFloat("pointLights[3].constant", 1.0f);
    m_LightingShader->setFloat("pointLights[3].linear", 0.09f);
    m_LightingShader->setFloat("pointLights[3].quadratic", 0.032f);

    m_LightingShader->setVec3("spotLight.position", m_MainCamera.getPosition());
    m_LightingShader->setVec3("spotLight.direction", m_MainCamera.getFront());
    m_LightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    m_LightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    m_LightingShader->setFloat3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    m_LightingShader->setFloat3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    m_LightingShader->setFloat3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    m_LightingShader->setFloat("spotLight.constant", 1.0f);
    m_LightingShader->setFloat("spotLight.linear", 0.09f);
    m_LightingShader->setFloat("spotLight.quadratic", 0.017f);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_MainCamera.getFov()), m_ResolutionX / m_ResolutionY, 0.1f, 100.0f);
    m_LightingShader->setMat4("projection", projectionMatrix);

    glm::mat4 viewMatrix = m_MainCamera.getViewMatrix();
    m_LightingShader->setMat4("view", viewMatrix);
    m_LightingShader->setVec3("viewPos", m_MainCamera.getPosition());

    for (int i = 0; i < 10; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, m_CubePositions[i]);
        float angle = 20.0f * i;
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_LightingShader->setMat4("model", modelMatrix);

        m_Cube->draw();
    }

    for (int i = 0; i < 3; i++)
    {
        glm::mat4 lightSourceModelMatrix = glm::mat4(1.0f);
        lightSourceModelMatrix = glm::translate(lightSourceModelMatrix, m_PointLightPositions[i]);
        lightSourceModelMatrix = glm::scale(lightSourceModelMatrix, glm::vec3(0.2f));
        m_LightSourceShader->use();
        m_LightSourceShader->setMat4("model", lightSourceModelMatrix);
        m_LightSourceShader->setMat4("view", viewMatrix);
        m_LightSourceShader->setMat4("projection", projectionMatrix);
        m_LightSourceShader->setVec3("lightColor", m_LightColors[i]);

        m_LightSource->draw();
    }

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Hephaestus");
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Press F1 to toggle UI interaction mode");
    ImGui::Text("UI mode: %s", m_UiInteractionMode ? "ON" : "OFF");
    if (ImGui::Checkbox("Wireframe", &m_WireframeMode))
        glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
    ImGui::Checkbox("Show ImGui demo", &m_ShowImGuiDemoWindow);
    ImGui::End();

    if (m_ShowImGuiDemoWindow)
        ImGui::ShowDemoWindow(&m_ShowImGuiDemoWindow);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CubeScene::Unload()
{
    m_MetallicContainerSpecular.reset();
    m_MetallicContainer.reset();
    m_LightSourceShader.reset();
    m_LightingShader.reset();
    m_LightSource.reset();
    m_Cube.reset();
}

void CubeScene::HandleKey(GLFWwindow* window, int key, int action)
{
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
    {
        m_UiInteractionMode = !m_UiInteractionMode;
        glfwSetInputMode(window, GLFW_CURSOR, m_UiInteractionMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        if (!m_UiInteractionMode)
            m_MainCamera.resetMouseCapture();
        return;
    }

    if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
    {
        m_WireframeMode = !m_WireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
    }
}

void CubeScene::HandleMouseMove(double xpos, double ypos)
{
    if (m_UiInteractionMode)
        return;

    m_MainCamera.processMouseMovement(xpos, ypos);
}

void CubeScene::HandleScroll(double xoffset, double yoffset)
{
    if (m_UiInteractionMode)
        return;

    m_MainCamera.handleZoom(xoffset, yoffset);
}

void CubeScene::HandleResize(int width, int height)
{
    m_ResolutionX = static_cast<float>(width);
    m_ResolutionY = static_cast<float>(height);
}

void CubeScene::ProcessMovementInput()
{
    if (glfwGetKey(m_Window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_MainCamera.move(render::CameraMovement::FORWARD, m_DeltaTime);

    if (glfwGetKey(m_Window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_MainCamera.move(render::CameraMovement::BACKWARD, m_DeltaTime);

    if (glfwGetKey(m_Window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_MainCamera.move(render::CameraMovement::LEFT, m_DeltaTime);

    if (glfwGetKey(m_Window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_MainCamera.move(render::CameraMovement::RIGHT, m_DeltaTime);
}

}
