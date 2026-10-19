#pragma once
#include <filesystem>
#include <ThirdParty/glm/glm.hpp>

class Shader
{
	unsigned int m_ProgramID;
	bool m_Initialized = false;

public:
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	void use();
	unsigned int getProgramID() { return m_ProgramID; }

	//Utility functions
	void setInt(const std::string& name, int i);
	void setFloat3(const std::string& name, float x, float y, float z) const;
	void setFloat4(const std::string& name, float x, float y, float z, float a) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;
	void setMat4(const std::string& name, const glm::mat4& matrix) const;
};