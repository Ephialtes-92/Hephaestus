#include <ThirdParty/glad/glad.h>

#include <hf/scene/CubeSceneGrey.h>

#include <filesystem>
#include <memory>
#include <utility>
#include <vector>

#include <config.h>

#include <ThirdParty/glm/gtc/matrix_transform.hpp>

#include <hf/render/window/window.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

constexpr int NUM_POINT_LIGHTS = 0;
constexpr bool SPOTLIGHT_ENABLED = false;

namespace
{
    std::filesystem::path shaderDirectory = SHADERS_DIR;
}

namespace hf::scene
{

CubeSceneGrey::CubeSceneGrey(render::Window& window):
    m_Window(window)
{
}

void CubeSceneGrey::Load()
{
    std::vector<float> cubeVertices3d = {
        //position
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    std::vector<float> planeVertices = {
         5.0f, -0.5f,  5.0f,
        -5.0f, -0.5f,  5.0f,
        -5.0f, -0.5f, -5.0f,

         5.0f, -0.5f,  5.0f,
        -5.0f, -0.5f, -5.0f,
         5.0f, -0.5f, -5.0f
    };

    std::vector<float> cubeNormals = {
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,

        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,

        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,

         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,

         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f, 
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f
    };

    std::vector<float> planeNormals = {
        0.0f, 0.0f,  0.0f,
        0.0f, 0.0f,  0.0f,
        0.0f, 0.0f,  0.0f,

        0.0f, 0.0f,  0.0f,
        0.0f, 0.0f,  0.0f,
        0.0f, 0.0f,  0.0f
    };


    std::vector<float> cubeTexCoords = {
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    std::vector<float> planeTexCoords = {
        2.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 2.0f,

        2.0f, 0.0f,
        0.0f, 2.0f,
        2.0f, 2.0f
    };

	//Create the textures
    std::vector<std::shared_ptr<render::Texture>> cubeTextures;
    cubeTextures.push_back(std::make_shared<render::Texture>("marble.jpg", "", render::TextureType::Diffuse));
    //textures.push_back(std::make_shared<render::Texture>("container2_specular.png", "", render::TextureType::Specular));

	//Construct the cube mesh
    std::vector<render::Vertex> vertices;
    vertices.reserve(cubeVertices3d.size() / 3);
	std::vector<unsigned int> indices {};
    for (std::size_t i = 0; i < cubeVertices3d.size() / 3; ++i)
    {
        vertices.push_back({
            glm::vec3(cubeVertices3d[i * 3], cubeVertices3d[i * 3 + 1], cubeVertices3d[i * 3 + 2]),
            glm::vec3(cubeNormals[i * 3], cubeNormals[i * 3 + 1], cubeNormals[i * 3 + 2]),
            glm::vec2(cubeTexCoords[i * 2], cubeTexCoords[i * 2 + 1])
        });
    }
    m_Cube = std::make_unique<render::Mesh>(vertices, indices, std::move(cubeTextures));

    ////Construct the plane mesh
    std::vector<std::shared_ptr<render::Texture>> planeTextures;
    planeTextures.push_back(std::make_shared<render::Texture>("metal.png", "", render::TextureType::Diffuse));
	planeTextures[0]->setWrapping(GL_REPEAT, GL_REPEAT);  

    std::vector<render::Vertex> planes_vertices;
    planes_vertices.reserve(planeVertices.size() / 3);
    std::vector<unsigned int> plane_indices{};
    for (std::size_t i = 0; i < planeVertices.size() / 3; ++i)
    {
        planes_vertices.push_back({
            glm::vec3(planeVertices[i * 3], planeVertices[i * 3 + 1], planeVertices[i * 3 + 2]),
            glm::vec3(planeNormals[i * 3], planeNormals[i * 3 + 1], planeNormals[i * 3 + 2]),
            glm::vec2(planeTexCoords[i * 2], planeTexCoords[i * 2 + 1])
            });
    }
    m_Plane = std::make_unique<render::Mesh>(planes_vertices, plane_indices, std::move(planeTextures));


	//no texture needed here
    m_LightSource = std::make_unique<render::Mesh>(vertices, indices, std::vector<std::shared_ptr<render::Texture>>{});

    m_LightingShader = std::make_unique<render::Shader>(
        shaderDirectory / "vertexShaderforLight.vert",
        shaderDirectory / "fragmentShaderforLight.frag"
    );
    m_LightSourceShader = std::make_unique<render::Shader>(
        shaderDirectory / "vertexShaderforLightSource.vert",
        shaderDirectory / "fragmentShaderforLightSource.frag"
    );

    m_LightingShader->use();

}

void CubeSceneGrey::Update(float deltaTime)
{
    m_DeltaTime = deltaTime;

    m_Window.processInput();
    const ImGuiIO& io = ImGui::GetIO();
    if (!m_UiInteractionMode && !io.WantCaptureKeyboard)
        ProcessMovementInput();
}

void CubeSceneGrey::Render()
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

    //m_LightingShader->setVec3("lightColor", glm::vec3(1.0f));

    m_LightingShader->setFloat3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
    m_LightingShader->setVec3("directionalLight.ambient", glm::vec3(1.0f));
    m_LightingShader->setVec3("directionalLight.diffuse", glm::vec3(1.0f));
    m_LightingShader->setVec3("directionalLight.specular", glm::vec3(0.0f));

	m_LightingShader->setInt("pointLightCount", NUM_POINT_LIGHTS);
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

    m_LightingShader->setBool("spotlightEnabled", SPOTLIGHT_ENABLED);
    if (SPOTLIGHT_ENABLED)
    {
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
    }

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_MainCamera.getFov()), m_ResolutionX / m_ResolutionY, 0.1f, 100.0f);
    m_LightingShader->setMat4("projection", projectionMatrix);

    glm::mat4 viewMatrix = m_MainCamera.getViewMatrix();
    m_LightingShader->setMat4("view", viewMatrix);
    m_LightingShader->setVec3("viewPos", m_MainCamera.getPosition());

	//Draw the cubes
    for (int i = 0; i < m_CubePositions->length() - 1; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, m_CubePositions[i]);
        //float angle = 20.0f * i;
        //modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_LightingShader->setMat4("model", modelMatrix);

        m_Cube->draw(*m_LightingShader);
    }

    //Draw the planes
    glm::mat4 planeModelMatrix = glm::mat4(1.0f);
    //planeModelMatrix = glm::translate(modelMatrix, m_CubePositions[i]);
    //float angle = 20.0f * i;
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    m_LightingShader->setMat4("model", planeModelMatrix);

    m_Plane->draw(*m_LightingShader);
 


	//Draw the light sources
    for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        glm::mat4 lightSourceModelMatrix = glm::mat4(1.0f);
        lightSourceModelMatrix = glm::translate(lightSourceModelMatrix, m_PointLightPositions[i]);
        lightSourceModelMatrix = glm::scale(lightSourceModelMatrix, glm::vec3(0.2f));
        m_LightSourceShader->use();
        m_LightSourceShader->setMat4("model", lightSourceModelMatrix);
        m_LightSourceShader->setMat4("view", viewMatrix);
        m_LightSourceShader->setMat4("projection", projectionMatrix);
        m_LightSourceShader->setVec3("lightColor", m_LightColors[i]);

        m_LightSource->draw(*m_LightSourceShader);
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

void CubeSceneGrey::Unload()
{
    m_LightSourceShader.reset();
    m_LightingShader.reset();
    m_LightSource.reset();
    m_Cube.reset();
}

void CubeSceneGrey::HandleKey(GLFWwindow* window, int key, int action)
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

void CubeSceneGrey::HandleMouseMove(double xpos, double ypos)
{
    if (m_UiInteractionMode)
        return;

    m_MainCamera.processMouseMovement(xpos, ypos);
}

void CubeSceneGrey::HandleScroll(double xoffset, double yoffset)
{
    if (m_UiInteractionMode)
        return;

    m_MainCamera.handleZoom(xoffset, yoffset);
}

void CubeSceneGrey::HandleResize(int width, int height)
{
    m_ResolutionX = static_cast<float>(width);
    m_ResolutionY = static_cast<float>(height);
}

void CubeSceneGrey::ProcessMovementInput()
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
