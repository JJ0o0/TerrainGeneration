#pragma once

#include <TerrainGeneration/graphics/Mesh.hpp>
#include <TerrainGeneration/graphics/Shader.hpp>

namespace JJ0o0::TerrainGeneration::Graphics {
inline constexpr int CHUNK_SIZE = 64;

class Chunk {
public:
  Chunk(int chunkX, int chunkZ, int size, int scale, float heightMultiplier,
        float seedX, float seedZ);
  ~Chunk();

  void render(Shader &shader);

  int getChunkX() const { return m_chunkX; }
  int getChunkZ() const { return m_chunkZ; }
  bool isReady() const { return m_mesh != nullptr; }

  void uploadToGPU();

private:
  int m_chunkX, m_chunkZ, m_size, m_scale;
  float m_seedX, m_seedZ, m_heightMultiplier;

  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;
  bool m_generated = false;
  Mesh *m_mesh;

  float sampleHeight(float x, float z);
  void generate();
};
} // namespace JJ0o0::TerrainGeneration::Graphics