#include <TerrainGeneration/graphics/Mesh.hpp>
#include <cstddef>

namespace JJ0o0::TerrainGeneration::Graphics {
Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices)
    : m_indexCount(indices.size()) {
  m_id = init(vertices, indices);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &m_id);
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
}

GLuint Mesh::init(const std::vector<Vertex> &vertices,
                  const std::vector<uint32_t> &indices) {
  // Vertex Array
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Vertex Buffer
  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  // Element Buffer
  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  // POSIÇÃO
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  // NORMAL
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  // TEXCOORD
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, textureCoords));
  glEnableVertexAttribArray(2);

  return VAO;
}
} // namespace JJ0o0::TerrainGeneration::Graphics