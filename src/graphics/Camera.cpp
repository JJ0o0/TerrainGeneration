#include <TerrainGeneration/graphics/Camera.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace JJ0o0::TerrainGeneration::Graphics {
glm::mat4 Camera::getView() const {
  return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjection() const {
  return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

} // namespace JJ0o0::TerrainGeneration::Graphics