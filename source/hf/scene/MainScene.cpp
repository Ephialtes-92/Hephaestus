#include <config.h>
#include <hf/scene/MainScene.h>
#include <hf/render/geometry/model.h>
#include <hf/render/window/window.h>

#include <memory>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


namespace hf::scene {
	MainScene::MainScene(render::Window& window) :
		m_Window(window)
	{
	}

	MainScene::~MainScene() = default;

	void MainScene::Load()
	{
		//Compile the shaders
		m_LightingShader = std::make_unique<render::Shader>(
			std::filesystem::path(SHADERS_DIR) / "vertexShaderforLight.vert",
			std::filesystem::path(SHADERS_DIR) / "fragmentShaderforLight.frag"
		);

		//Load the model(s)
		m_Models.push_back(std::make_unique<hf::render::Model>(std::filesystem::path(MODELS_DIR) / "backpack" / "backpack.obj"));

		//Not rendering the light source for now
		//m_LightSourceShader = std::make_unique<render::Shader>(
		//	std::filesystem::path(SHADERS_DIR) / "vertexShaderforLightSource.vert",
		//	std::filesystem::path(SHADERS_DIR) / "fragmentShaderforLightSource.frag"
		//);

	}

	void MainScene::Update(float deltaTime)
	{
		m_DeltaTime = deltaTime;
		m_Window.processInput();

		//Does the Imgui stuff belong here? :hmmmm
		const ImGuiIO& io = ImGui::GetIO();
		if (!m_UiInteractionMode && !io.WantCaptureKeyboard)
			ProcessMovementInput();

	}

	void MainScene::Render()
	{
		//Imgui stuff 
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
		m_LightingShader->setVec3("directionalLight.ambient", glm::vec3(0.0f));
		m_LightingShader->setVec3("directionalLight.diffuse", glm::vec3(1.0f));
		m_LightingShader->setVec3("directionalLight.specular", glm::vec3(1.0f));

		//point lights
		m_LightingShader->setInt("pointLightCount", 1);
		m_LightingShader->setVec3("pointLights[0].position", m_PointLightPositions[0]);
		m_LightingShader->setVec3("pointLights[0].ambient", ambientColor[0]);
		m_LightingShader->setVec3("pointLights[0].diffuse", diffuseColor[0]);
		m_LightingShader->setVec3("pointLights[0].specular", glm::vec3(1.0f));
		m_LightingShader->setFloat("pointLights[0].constant", 1.0f);
		m_LightingShader->setFloat("pointLights[0].linear", 0.09f);
		m_LightingShader->setFloat("pointLights[0].quadratic", 0.032f);

		//What happens if we have no spot light?
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

		//Set the camera matrices
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_MainCamera.getFov()), m_ResolutionX / m_ResolutionY, 0.1f, 100.0f);
		m_LightingShader->setMat4("projection", projectionMatrix);

		glm::mat4 viewMatrix = m_MainCamera.getViewMatrix();
		m_LightingShader->setMat4("view", viewMatrix);
		m_LightingShader->setVec3("viewPos", m_MainCamera.getPosition());

		//Draw the models
		m_LightingShader->setMat4("model", glm::mat4(1.0f));
		for (const auto& model : m_Models)
		{
			model->draw(*m_LightingShader);
		}

		//More Imgui stuff
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

	void MainScene::Unload()
	{
		//m_LightSourceShader.reset();
		m_LightingShader.reset();
		for (auto& model : m_Models)
		{
			model.reset();
		}

	}

	void MainScene::HandleKey(GLFWwindow* window, int key, int action)
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

	void MainScene::HandleMouseMove(double xpos, double ypos)
	{
		if (m_UiInteractionMode)
			return;

		m_MainCamera.processMouseMovement(xpos, ypos);
	}

	void MainScene::HandleScroll(double xoffset, double yoffset)
	{
		if (m_UiInteractionMode)
			return;

		m_MainCamera.handleZoom(xoffset, yoffset);
	}

	void MainScene::HandleResize(int width, int height)
	{
		m_ResolutionX = static_cast<float>(width);
		m_ResolutionY = static_cast<float>(height);
	}

	void MainScene::ProcessMovementInput()
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



} // namespace hf::scene
