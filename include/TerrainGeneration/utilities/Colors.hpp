#pragma once

#include <SDL3/SDL_pixels.h>
#include <glm/ext/vector_float4.hpp>

namespace JJ0o0::TerrainGeneration::Utils {
inline constexpr glm::vec4 BACKGROUND_GL = {0.53f, 0.70f, 0.85f, 1.0f};

inline constexpr SDL_Color BACKGROUND_SDL = {33, 33, 33, 255};
inline constexpr SDL_Color TEXT_SDL = {255, 255, 255, 255};
} // namespace JJ0o0::TerrainGeneration::Utils