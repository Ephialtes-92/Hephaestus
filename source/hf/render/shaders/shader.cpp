#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <hf/render/shaders/shader.h>
#include <ThirdParty/glad/glad.h>
#include <filesystem>
#include <ios>
#include <string>
#include <ThirdParty/glm/fwd.hpp>
#include <ThirdParty/glm/gtc/type_ptr.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace
{
    void logShaderMessage(const std::string& message)
    {
        std::cerr << message << '\n';
#ifdef _WIN32
        OutputDebugStringA((message + "\n").c_str());
#endif
    }

    bool readShaderSource(const std::filesystem::path& path, std::string& source)
    {
        std::ifstream file(path);
        if (!file)
        {
            logShaderMessage("[Shader] Failed to open file: " + path.string());
            return false;
        }

        std::ostringstream stream;
        stream << file.rdbuf();
        if (file.bad())
        {
            logShaderMessage("[Shader] Failed to read file: " + path.string());
            return false;
        }

        source = stream.str();
        return true;
    }

    unsigned int compileShader(unsigned int type, const std::string& source, const std::filesystem::path& path, const char* stage)
    {
        const unsigned int shader = glCreateShader(type);
        const char* sourceText = source.c_str();
        glShaderSource(shader, 1, &sourceText, nullptr);
        glCompileShader(shader);

        int success = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1)
        {
            std::vector<char> infoLog(static_cast<size_t>(logLength));
            glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
            logShaderMessage(std::string("[Shader][") + (success ? "Info" : "Error") + "][" + stage + "] " + path.string() + ":\n" + infoLog.data());
        }
        if (!success)
        {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }
}

namespace hf::render
{
Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    if (!readShaderSource(vertexPath, vertexCode) || !readShaderSource(fragmentPath, fragmentCode))
        return;

    const unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode, vertexPath, "Vertex");
    const unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode, fragmentPath, "Fragment");
    if (!vertex || !fragment)
    {
        if (vertex)
            glDeleteShader(vertex);
        if (fragment)
            glDeleteShader(fragment);
        return;
    }

    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertex);
    glAttachShader(m_ProgramID, fragment);
    glLinkProgram(m_ProgramID);

    int success = GL_FALSE;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
    int logLength = 0;
    glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1)
    {
        std::vector<char> infoLog(static_cast<size_t>(logLength));
        glGetProgramInfoLog(m_ProgramID, logLength, nullptr, infoLog.data());
        logShaderMessage(std::string("[Shader][") + (success ? "Info" : "Error") + "][Link] " + vertexPath.string() + " + " + fragmentPath.string() + ":\n" + infoLog.data());
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (!success)
    {
        glDeleteProgram(m_ProgramID);
        m_ProgramID = 0;
        return;
    }

    m_Initialized = true;
}

void Shader::use()
{
    if (m_Initialized && m_ProgramID)
    {
        glUseProgram(m_ProgramID);
    }
    //assert if we are trying to use an unitialized shader
}

int Shader::uniformLocation(const std::string& name) const
{
    if (!m_Initialized || !m_ProgramID)
        return -1;

    const int location = glGetUniformLocation(m_ProgramID, name.c_str());
    if (location == -1 && m_WarnedUniforms.insert(name).second)
        logShaderMessage("[Shader][Warning] Uniform '" + name + "' is missing or optimized out in program " + std::to_string(m_ProgramID));

    return location;
}

void Shader::setBool(const std::string& name, bool value) const
{
    const int location = uniformLocation(name);
    if (location != -1)
        glUniform1i(location, value ? 1 : 0);
}

void Shader::setInt(const std::string& name, int i)
{
    const int location = uniformLocation(name);
    if (location != -1)
        glUniform1i(location, i);
}

void Shader::setFloat(const std::string& name, float f) const
{
    const int location = uniformLocation(name);
    if (location != -1)
        glUniform1f(location, f);
}

void Shader::setFloat3(const std::string& name, float x, float y, float z) const
{
    const int location = uniformLocation(name);
    if (location != -1)
        glUniform3f(location, x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
    const int location = uniformLocation(name);
    if (location != -1)
        glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::setFloat4(const std::string& name, float x, float y, float z, float a) const
{
    const int location = uniformLocation(name);
    if (location != -1)
        glUniform4f(location, x, y, z, a);
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const
{
    const int location = uniformLocation(name);
    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
}
