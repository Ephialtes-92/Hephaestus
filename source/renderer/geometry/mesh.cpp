#include <renderer/geometry/mesh.h>

Mesh::Mesh(const float* vertices, GLsizeiptr vertexBufferSize, const unsigned int* indices, GLsizeiptr indexBufferSize)
{
	//First we create a VAO --> Vertex Array Object
	//We can reuse this betwen triangles and rectangles as it stores both VBOs and EBOs
	glGenVertexArrays(1, &m_VAO);

	//Bind VAO
	glBindVertexArray(m_VAO);

	//We need a VBO too to store the vertices
	//the EBO only stores the indices of said vertices
	glGenBuffers(1, &m_VBO);

	//Bind the verices to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STATIC_DRAW);

	//Create the EBO
	glGenBuffers(1, &m_EBO);

	//Bind the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices, GL_STATIC_DRAW);

	//Link the vertex attributes
	//Position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Normal vector data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	m_IndexCount = static_cast<GLsizei>(indexBufferSize / sizeof(unsigned int));

	//Reset the OpenGL state
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_EBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
}

void Mesh::draw()
{
	glBindVertexArray(m_VAO);
	if (m_IndexCount != 0)
	{
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}