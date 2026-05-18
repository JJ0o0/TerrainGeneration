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
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  int resolution = 20;
  float step = size * 2.0f / resolution;

  for (int z = 0; z <= resolution; z++) {
    for (int x = 0; x <= resolution; x++) {
      float wx = -size + x * step;
      float wz = -size + z * step;

      vertices.push_back({{wx, m_height, wz},
                          {0.0f, 1.0f, 0.0f},
                          {(float)x / resolution, (float)z / resolution}});
    }
  }

  int w = resolution + 1;
  for (int z = 0; z < resolution; z++) {
    for (int x = 0; x < resolution; x++) {
      unsigned int A = z * w + x;
      unsigned int B = z * w + x + 1;
      unsigned int C = (z + 1) * w + x;
      unsigned int D = (z + 1) * w + x + 1;

      indices.push_back(A);
      indices.push_back(C);
      indices.push_back(B);
      indices.push_back(B);
      indices.push_back(C);
      indices.push_back(D);
    }
  }

  m_mesh = new Mesh(vertices, indices);
}

} // namespace JJ0o0::TerrainGeneration::Graphics