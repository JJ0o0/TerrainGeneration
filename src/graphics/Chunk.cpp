#include <TerrainGeneration/graphics/Chunk.hpp>
#include <TerrainGeneration/graphics/Vertex.hpp>
#include <TerrainGeneration/utilities/Log.hpp>
#include <TerrainGeneration/utilities/Random.hpp>
#include <stb_perlin.h>
#include <vector>

using namespace JJ0o0::TerrainGeneration::Utils;

namespace JJ0o0::TerrainGeneration::Graphics {
Chunk::Chunk(int chunkX, int chunkZ, int size, int scale,
             float heightMultiplier, float seedX, float seedZ)
    : m_chunkX(chunkX), m_chunkZ(chunkZ), m_size(size), m_scale(scale),
      m_heightMultiplier(heightMultiplier), m_seedX(seedX), m_seedZ(seedZ) {
  generate();
}

Chunk::~Chunk() {
  if (m_mesh) {
    delete m_mesh;
    m_mesh = nullptr;
  }
}

void Chunk::render(Shader &shader) {
  shader.bind();
  m_mesh->bind();
  m_mesh->draw();
}

float Chunk::sampleHeight(float x, float z) {
  float height = 0.0f;
  float amplitude = 1.0f;
  float frequency = 0.002f;
  float total = 0.0f;

  for (int i = 0; i < 8; i++) {
    height += stb_perlin_noise3((x + m_seedX) * frequency, 0.0f,
                                (z + m_seedZ) * frequency, 0, 0, 0) *
              amplitude;
    total += amplitude;
    amplitude *= 0.5f;
    frequency *= 2.0f;
  }

  return (height / total) * m_heightMultiplier;
}

void Chunk::generate() {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (int z = 0; z < m_size; z++) {
    for (int x = 0; x < m_size; x++) {
      float worldX = (m_chunkX * (m_size - 1) + x) * m_scale;
      float worldZ = (m_chunkZ * (m_size - 1) + z) * m_scale;

      Vertex vertex = {{worldX, sampleHeight(worldX, worldZ), worldZ},
                       {.0f, 1.0f, .0f},
                       {(float)x / m_size, (float)z / m_size}};
      vertices.push_back(vertex);
    }
  }

  for (int z = 0; z < m_size; z++) {
    for (int x = 0; x < m_size; x++) {
      float worldX = (m_chunkX * (m_size - 1) + x) * m_scale;
      float worldZ = (m_chunkZ * (m_size - 1) + z) * m_scale;

      float hL = sampleHeight(worldX - m_scale, worldZ);
      float hR = sampleHeight(worldX + m_scale, worldZ);
      float hD = sampleHeight(worldX, worldZ - m_scale);
      float hU = sampleHeight(worldX, worldZ + m_scale);

      glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
      vertices[z * m_size + x].normal = normal;
    }
  }

  for (int z = 0; z < m_size - 1; z++) {
    for (int x = 0; x < m_size - 1; x++) {
      unsigned int A = z * m_size + x;
      unsigned int B = z * m_size + x + 1;
      unsigned int C = (z + 1) * m_size + x;
      unsigned int D = (z + 1) * m_size + x + 1;

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