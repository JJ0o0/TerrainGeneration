#pragma once

#include <TerrainGeneration/graphics/Vertex.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace JJ0o0::TerrainGeneration::Graphics {
class Mesh {
public:
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices);
  ~Mesh();

  void bind() const { glBindVertexArray(m_id); }
  void unbind() const { glBindVertexArray(0); };
  void draw() const {
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
  }

private:
  GLuint m_id;
  GLuint m_VBO, m_EBO;
  GLsizei m_indexCount;

  GLuint init(const std::vector<Vertex> &vertices,
              const std::vector<unsigned int> &indices);
};
} // namespace JJ0o0::TerrainGeneration::Graphics