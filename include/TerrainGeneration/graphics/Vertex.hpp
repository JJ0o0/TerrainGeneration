#pragma once

#include <glm/glm.hpp>

namespace JJ0o0::TerrainGeneration::Graphics {
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoords;
};
} // namespace JJ0o0::TerrainGeneration::Graphics