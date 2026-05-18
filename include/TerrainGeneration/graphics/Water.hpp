#pragma once

#include <TerrainGeneration/graphics/Mesh.hpp>
#include <TerrainGeneration/graphics/Shader.hpp>

namespace JJ0o0::TerrainGeneration::Graphics {
class Water {
public:
  Water(float size, float height);
  ~Water();

  void render(Shader &shader);

private:
  Mesh *m_mesh;
  float m_height;

  void generate(float size);
};
} // namespace JJ0o0::TerrainGeneration::Graphics