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

private:
  int m_chunkX, m_chunkZ, m_size, m_scale;
  float m_seedX, m_seedZ, m_heightMultiplier;

  Mesh *m_mesh;

  float sampleHeight(float x, float z);
  void generate();
};
} // namespace JJ0o0::TerrainGeneration::Graphics