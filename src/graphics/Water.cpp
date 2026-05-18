#include <TerrainGeneration/graphics/Water.hpp>

namespace JJ0o0::TerrainGeneration::Graphics {
Water::Water(float size, float height) : m_height(height) { generate(size); }
Water::~Water() {
  if (m_mesh) {
    delete m_mesh;
    m_mesh = nullptr;
  }
}

void Water::render(Shader &shader) {
  m_mesh->bind();
  m_mesh->draw();
}

void Water::generate(float size) {
  std::vector<Vertex> vertices = {
      {{-size, m_height, -size}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
      {{size, m_height, -size}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{size, m_height, size}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
      {{-size, m_height, size}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
  };

  std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

  m_mesh = new Mesh(vertices, indices);
}

} // namespace JJ0o0::TerrainGeneration::Graphics