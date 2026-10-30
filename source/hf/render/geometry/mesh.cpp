#include <hf/render/geometry/mesh.h>
#include <utility>

namespace hf::render
{
Mesh::Mesh(std::vector <Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<hf::render::Texture>> textures)
    : m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_Textures(std::move(textures))
{
    setupMesh();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_EBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
}

Mesh::Mesh(Mesh&& other) noexcept
	: m_Vertices(std::move(other.m_Vertices)),
	  m_Indices(std::move(other.m_Indices)),
	  m_Textures(std::move(other.m_Textures)),
	  m_VAO(std::exchange(other.m_VAO, 0)),
	  m_VBO(std::exchange(other.m_VBO, 0)),
	  m_EBO(std::exchange(other.m_EBO, 0)),
	  m_IndexCount(std::exchange(other.m_IndexCount, 0))
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(1, &m_EBO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_VAO);
		m_Vertices = std::move(other.m_Vertices);
		m_Indices = std::move(other.m_Indices);
		m_Textures = std::move(other.m_Textures);
		m_VAO = std::exchange(other.m_VAO, 0);
		m_VBO = std::exchange(other.m_VBO, 0);
		m_EBO = std::exchange(other.m_EBO, 0);
		m_IndexCount = std::exchange(other.m_IndexCount, 0);
	}
	return *this;
}

void Mesh::setupMesh()
{
	//First we create a VAO --> Vertex Array Object
	//We can reuse this between triangles and rectangles as it stores both VBOs and EBOs
	glGenVertexArrays(1, &m_VAO);

	//Bind VAO
	glBindVertexArray(m_VAO);

	//We need a VBO too to store the vertices
	//the EBO only stores the indices of said vertices
	glGenBuffers(1, &m_VBO);

	//Bind the verices to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

	//Create the EBO
	glGenBuffers(1, &m_EBO);

	//Bind the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	//Link the vertex attributes
	//Position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	//Normal vector data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	//Texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	//Reset the OpenGL state
	glBindVertexArray(0);

}

void Mesh::draw(hf::render::Shader& shader)
{
	//Bind the textures
    //only works for 1 diffuse and 1 specular texture for now
	unsigned int diffuseCount = 1;
	unsigned int specularCount = 1;
    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {
        std::string number;
        std::string name;
        //glActiveTexture(GL_TEXTURE0 + i); 

		auto type = m_Textures[i]->getType();
		switch (type)
		{		
		case hf::render::TextureType::Diffuse:
            number = std::to_string(diffuseCount++);
			name = "diffuse";
			break;
		case hf::render::TextureType::Specular:
            number = std::to_string(specularCount++);
			name = "specular";
			break;
		default:
            //Wrong texture type for now, we only support diffuse and specular
			break;
		}

        shader.setInt("material." + name /*+ number*/, i);
		m_Textures[i]->bind(i);
    }

    //Draw the mesh
    glBindVertexArray(m_VAO);
	if (m_Indices.size() > 0)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
	}
	//Leave this here for now as a workaround for when we don't have an EBO (no vertices provided basically), but we should probably remove this later on
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
    glBindVertexArray(0);
}

}
