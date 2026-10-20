#include <fstream>
#include <sstream>
#include <iostream>

#include <renderer/shaders/shader.h>
#include <ThirdParty/glad/glad.h>
#include <filesystem>
#include <ios>
#include <string>
#include <ThirdParty/glm/fwd.hpp>
#include <ThirdParty/glm/gtc/type_ptr.hpp>

//Read content and compile shaders
Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    // ensure ifstream objects can throw exceptions:
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        std::stringstream vertexStringStream, fragmentStringStream;
        // read file's buffer contents into streams
        vertexStringStream << vertexFile.rdbuf();
        fragmentStringStream << fragmentFile.rdbuf();
        // close file handlers
        vertexFile.close();
        fragmentFile.close();
        // convert stream into string
        vertexCode = vertexStringStream.str();
        fragmentCode = fragmentStringStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertexCodeC = vertexCode.c_str();
    const char* fragmentCodeC = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCodeC, NULL);
    glCompileShader(vertex);

    //Check if shader compilation succeeded
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCodeC, NULL);
    glCompileShader(fragment);

    //Check if shader compilation succeeded
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 3. Attach and link shaders to the programm
    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertex);
    glAttachShader(m_ProgramID, fragment);
    glLinkProgram(m_ProgramID);

    //Check if the linking worked
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    m_Initialized = true;
    //We no longer need the shader objects after they have been linked
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use()
{
    if (m_Initialized && m_ProgramID)
    {
        glUseProgram(m_ProgramID);
    }
    //assert if we are trying to use an unitialized shader
}

//void Shader::setBool(const std::string& name, bool value) const
//{
//}

void Shader::setInt(const std::string& name, int i)
{
    int uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());
    // not found is -1 add a check here)
    //also check that the shader is properly initialized
    glUniform1i(uniformLocation, i);
}

void Shader::setFloat(const std::string& name, float f) const
{
    int uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());
    // not found is -1 add a check here)
    //also check that the shader is properly initialized
    glUniform1f(uniformLocation, f);
}

void Shader::setFloat3(const std::string& name, float x, float y, float z) const
{
    int uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());
    // not found is -1 add a check here)
    //also check that the shader is properly initialized
    glUniform3f(uniformLocation, x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
    int uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());
    // not found is -1 add a check here)
    //also check that the shader is properly initialized
    glUniform3f(uniformLocation, vec.x, vec.y, vec.z);
}

void Shader::setFloat4(const std::string& name, float x, float y, float z, float a) const
{
    int uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());
    // not found is -1 add a check here)
    //also check that the shader is properly initialized
    glUniform4f(uniformLocation, x, y, z, a);
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const
{
    int uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());
    // not found is -1 add a check here)
    //also check that the shader is properly initialized
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}
