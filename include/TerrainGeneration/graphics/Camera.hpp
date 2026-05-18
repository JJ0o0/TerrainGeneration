#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

namespace JJ0o0::TerrainGeneration::Graphics {
struct Camera {
  glm::vec3 position, front, up;
  float yaw, pitch, fov, aspectRatio, nearPlane, farPlane;

  glm::mat4 getView() const;
  glm::mat4 getProjection() const;
};
} // namespace JJ0o0::TerrainGeneration::Graphics