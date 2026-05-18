#pragma once

#include <TerrainGeneration/graphics/Mesh.hpp>
#include <TerrainGeneration/graphics/Shader.hpp>

namespace JJ0o0::TerrainGeneration::Graphics {
class Terrain {
public:
  Terrain(int width, int depth, int scale, float heightMultiplier);
  ~Terrain();

  void render(Shader &shader);

private:
  int m_width, m_depth;
  float m_scale, m_seedX, m_seedZ, m_heightMultiplier;

  Mesh *m_mesh;

  float sampleHeight(float x, float z);
  void generate();
};
} // namespace JJ0o0::TerrainGeneration::Graphics