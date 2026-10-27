#pragma once
#include <ThirdParty/glad/glad.h>

namespace hf::render
{
class Mesh {
public:
    Mesh(const float* vertices, GLsizeiptr vertexBufferSize, const unsigned int* indices = nullptr, GLsizeiptr indexBufferSize = 0);
    ~Mesh();

    // GPU resource ownership should not be copied accidentally.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw();
private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    GLsizei m_IndexCount = 0;

};
}
