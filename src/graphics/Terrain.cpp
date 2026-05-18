#include <TerrainGeneration/graphics/Terrain.hpp>
#include <TerrainGeneration/graphics/Vertex.hpp>
#include <TerrainGeneration/utilities/Log.hpp>
#include <TerrainGeneration/utilities/Random.hpp>
#include <stb_perlin.h>
#include <vector>

using namespace JJ0o0::TerrainGeneration::Utils;

namespace JJ0o0::TerrainGeneration::Graphics {
Terrain::Terrain(int width, int depth, int scale)
    : m_width(width), m_depth(depth), m_scale(scale) {
  m_seedX = Utils::randomFloat(.0f, 10000.0f);
  m_seedZ = Utils::randomFloat(.0f, 10000.0f);

  generate();
}

Terrain::~Terrain() {
  if (m_mesh) {
    delete m_mesh;
    m_mesh = nullptr;
  }
}

void Terrain::render(Shader &shader) {
  shader.bind();
  m_mesh->bind();
  m_mesh->draw();
}

float Terrain::sampleHeight(float x, float z) {
  float height = 0.0f;
  float amplitude = 1.0f;
  float frequency = 0.005f;
  float total = 0.0f;

  for (int i = 0; i < 6; i++) {
    height += stb_perlin_noise3((x + m_seedX) * frequency, 0.0f,
                                (z + m_seedZ) * frequency, 0, 0, 0) *
              amplitude;
    total += amplitude;
    amplitude *= 0.5f;
    frequency *= 2.0f;
  }

  return (height / total) * 40.0f;
}

void Terrain::generate() {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (int z = 0; z < m_depth; z++) {
    for (int x = 0; x < m_width; x++) {
      Vertex vertex = {{x * m_scale, sampleHeight(x, z), z * m_scale},
                       {.0f, 1.0f, .0f},
                       {(float)x / m_width, (float)z / m_depth}};
      vertices.push_back(vertex);
    }
  }

  for (int z = 0; z < m_depth; z++) {
    for (int x = 0; x < m_width; x++) {
      glm::vec3 normal;

      float hL = vertices[z * m_width + std::max(x - 1, 0)].position.y;
      float hR =
          vertices[z * m_width + std::min(x + 1, m_width - 1)].position.y;
      float hD = vertices[std::max(z - 1, 0) * m_width + x].position.y;
      float hU =
          vertices[std::min(z + 1, m_depth - 1) * m_width + x].position.y;

      normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
      vertices[z * m_width + x].normal = normal;
    }
  }

  for (int z = 0; z < m_depth - 1; z++) {
    for (int x = 0; x < m_width - 1; x++) {
      unsigned int A = z * m_width + x;
      unsigned int B = z * m_width + x + 1;
      unsigned int C = (z + 1) * m_width + x;
      unsigned int D = (z + 1) * m_width + x + 1;

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