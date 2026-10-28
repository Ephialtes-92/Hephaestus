#pragma once

#include <hf/render/textures/texture.h>
#include <hf/render/shaders/shader.h>
#include <ThirdParty/glad/glad.h>
#include <vector>

namespace hf::render
{

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class Mesh {
    public:
        Mesh(std::vector <Vertex> vertices, std::vector<unsigned int> indices, std::vector<hf::render::Texture> textures);
        ~Mesh();

        // GPU resource ownership should not be copied accidentally.
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        //Move mesh seetup here, what would be the most efficient way to pass the data? references or std::move? I think std::move is the best option here, as we don't need the data after the mesh is created.
        void setupMesh();
        //Should we be passing the shader here?
        void draw(hf::render::Shader& shader);
    private:
        //Mesh data
        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<hf::render::Texture> m_Textures;

        //render data
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        unsigned int m_EBO = 0;

        GLsizei m_IndexCount = 0;

    };
}
