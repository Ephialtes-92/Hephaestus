#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GLFW_INCLUDE_NONE
#include <ThirdParty/GLFW/glfw3.h>
#include <ThirdParty/glad/glad.h>

#include <renderer/window/window.h>
#include <renderer/shaders/shader.h>
#include <renderer/textures/texture.h>
#include <renderer/geometry/mesh.h>
#include <filesystem>
#include <config.h>

//Math stuff
#include <ThirdParty/glm/glm.hpp>
#include <ThirdParty/glm/gtc/matrix_transform.hpp>

//camera
#include <renderer/camera/Camera.h>

//DEBUG
bool wireframeMode = false;

//Forward declarations
void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void renderLoop(Window& window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

std::filesystem::path shaderDirectory = SHADERS_DIR;

//Window
void processInput(GLFWwindow* window);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Camera 
Camera mainCamera;


int main()
{

    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);

    //Init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window(800, 600, "OpenGL Renderer");

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
    glViewport(0, 0, 800, 600);

    //Enable depth testing
    glEnable(GL_DEPTH_TEST);

    //Register GLFW callbacks
    glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);

    //Simple Render Loop
    renderLoop(window);

    return 0;

}

void renderLoop(Window& window)
{
    //An Element Buffer Object (EBO) is a buffer that OpenGL uses to decide what vertices to draw
    //This lets store only the unique vertices of an object we want to draw
    const float vertices[] = {
        // positions        // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left 
    };

    //3d rectangle
float vertices3d[] = {
    // positions          // normals           // texture coords
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

    glm::vec3 cubePositions[] = {
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

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

	glm::vec3 lightColors[] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.2f, 0.8f, 0.65f)
	};


    //Now we can define the triangles that make the rectange
    //Without having to store duplicate vertices
    const unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    Mesh rectangle(vertices, sizeof(vertices), indices, sizeof(indices));
    Mesh cube(vertices3d, sizeof(vertices3d));

	//Light source cube
    Mesh lightSource(vertices3d, sizeof(vertices3d));
    glm::vec3 lightPos(3.0f, 0.0f, 0.0f);

    glm::mat4 viewMatrix = mainCamera.getViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(mainCamera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

    Shader baseShader(
        shaderDirectory / "vertexShader.vert",
        shaderDirectory / "fragmentShader.frag"
    );
    Shader yellowShader(
        shaderDirectory / "vertexShader.vert",
        shaderDirectory / "fragmentShaderYellow.frag"
    );
    Shader blinkingShader(
        shaderDirectory / "vertexShader.vert",
        shaderDirectory / "uniformTest.frag"
    );
    Shader upsideBlinkingShader(
        shaderDirectory / "upsidedownVertexShader.vert",
        shaderDirectory / "uniformTest.frag"
    );
    Shader upsideDownOffsetYellowShader(
        shaderDirectory / "upsidedownOffsetVertexShader.vert",
        shaderDirectory / "fragmentShaderYellow.frag"
    );
    Shader weirdColorShader(
        shaderDirectory / "posOutToFragmentShader.vert",
        shaderDirectory / "inPositionFragmentShader.frag"
    );
    Shader textureShader(
        shaderDirectory / "vertexShaderforTexture.vert",
        shaderDirectory / "fragmentShaderforTexture.frag"
    );
    Shader lightingShader(
        shaderDirectory / "vertexShaderforLight.vert",
        shaderDirectory / "fragmentShaderforLight.frag"
    );
	Shader lightSourceShader(
		shaderDirectory / "vertexShaderforLightSource.vert",
		shaderDirectory / "fragmentShaderforLightSource.frag"
	);


    //Load the textures
    //Careful to construc/load textures first and then bind them to their corresponding units
    Texture metallicContainer("container2.png");
    Texture metallicContainer_specular("container2_specular.png");

	metallicContainer.bind(0);
    metallicContainer_specular.bind(1);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);


    while (!window.shouldClose())
    {
        //Frame times
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Input
        window.processInput();
        processInput(window.getWindow());

        //Render commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();

		//Material properties
        lightingShader.setFloat("material.shininess", 32.0f);

		//Light properties
        glm::vec3 lightColor = glm::vec3(1.0f);
        //lightColor.x = sin(glfwGetTime() * 2.0f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor[] = {
            lightColors[0] * glm::vec3(0.8f),
            lightColors[1] * glm::vec3(0.8f),
            lightColors[2] * glm::vec3(0.8f),
			lightColors[3] * glm::vec3(0.8f)
        };

        glm::vec3 ambientColor[] = {
            diffuseColor[0] * glm::vec3(0.0f),
            diffuseColor[1] * glm::vec3(0.0f),
            diffuseColor[2] * glm::vec3(0.0f),
			diffuseColor[3] * glm::vec3(0.0f)
        };

        lightingShader.setVec3("lightColor", lightColor);

        //hardcode light direction for directional light
        lightingShader.setFloat3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("directionalLight.ambient", glm::vec3(0.0f));
        lightingShader.setVec3("directionalLight.diffuse", glm::vec3(1.0f));
        lightingShader.setVec3("directionalLight.specular", glm::vec3(1.0f));


		//Point light properties
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", ambientColor[0]);
        lightingShader.setVec3("pointLights[0].diffuse", diffuseColor[0]);
        lightingShader.setVec3("pointLights[0].specular", glm::vec3(1.0f));
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", ambientColor[1]);
        lightingShader.setVec3("pointLights[1].diffuse", diffuseColor[1]);
        lightingShader.setVec3("pointLights[1].specular", glm::vec3(1.0f));
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);

        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", ambientColor[2]);
        lightingShader.setVec3("pointLights[2].diffuse", diffuseColor[2]);
        lightingShader.setVec3("pointLights[2].specular", glm::vec3(1.0f));
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09f);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032f);

        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", ambientColor[3]);
        lightingShader.setVec3("pointLights[3].diffuse", diffuseColor[3]);
        lightingShader.setVec3("pointLights[3].specular", glm::vec3(1.0f));
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09f);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032f);

		//Spotlight properties
		lightingShader.setVec3("spotLight.position", mainCamera.getPosition());
		lightingShader.setVec3("spotLight.direction", mainCamera.getFront());
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        lightingShader.setFloat3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setFloat3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.017f);


        projectionMatrix = glm::perspective(glm::radians(mainCamera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projectionMatrix);

        viewMatrix = mainCamera.getViewMatrix();
        //lightPos = glm::vec3(3.0f * sin(glfwGetTime()), 1.0f * cos(glfwGetTime()), 1.0f * cos(glfwGetTime()));
        lightingShader.setMat4("view", viewMatrix);
        lightingShader.setVec3("viewPos", mainCamera.getPosition());

        //Draw cubes in various positions
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            float angle = 20.0f * i;
            modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", modelMatrix);

            cube.draw();
        }

		//Draw the light source cubes
		for (int i = 0; i < 3; i++)
		{
			glm::mat4 lightSourceModelMatrix = glm::mat4(1.0f);
			lightSourceModelMatrix = glm::translate(lightSourceModelMatrix, pointLightPositions[i]);
			lightSourceModelMatrix = glm::scale(lightSourceModelMatrix, glm::vec3(0.2f)); // Make it smaller than the cubes
			lightSourceShader.use();
			lightSourceShader.setMat4("model", lightSourceModelMatrix);
			lightSourceShader.setMat4("view", viewMatrix);
			lightSourceShader.setMat4("projection", projectionMatrix);
			lightSourceShader.setVec3("lightColor", lightColors[i]);

			lightSource.draw();
		}

        window.onUpdate();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
    {
        if (!wireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframeMode = true;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframeMode = false;
        }

    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mainCamera.processMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mainCamera.handleZoom(xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 6.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mainCamera.move(CameraMovement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        mainCamera.move(CameraMovement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mainCamera.move(CameraMovement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mainCamera.move(CameraMovement::RIGHT, deltaTime);
    }
}