#pragma once

#include <SDL3/SDL_pixels.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace JJ0o0::TerrainGeneration::Utils {

inline constexpr glm::vec4 BACKGROUND_GL = {0.53f, 0.70f, 0.85f, 1.0f};
inline constexpr glm::vec3 SKY_DAY = {0.53f, 0.70f, 0.85f};
inline constexpr glm::vec3 SKY_SUNSET = {0.90f, 0.45f, 0.20f};
inline constexpr glm::vec3 SKY_NIGHT = {0.05f, 0.05f, 0.15f};

inline constexpr glm::vec3 LIGHT_DAY = {1.00f, 0.95f, 0.80f};
inline constexpr glm::vec3 LIGHT_SUNSET = {1.00f, 0.60f, 0.30f};
inline constexpr glm::vec3 LIGHT_NIGHT = {0.10f, 0.10f, 0.20f};

inline constexpr SDL_Color BACKGROUND_SDL = {33, 33, 33, 255};
inline constexpr SDL_Color TEXT_SDL = {255, 255, 255, 255};

} // namespace JJ0o0::TerrainGeneration::Utils