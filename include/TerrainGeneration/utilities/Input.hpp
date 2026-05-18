#pragma once

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>
#include <cstddef>

namespace JJ0o0::TerrainGeneration::Utils {
inline bool isKeyPressed(SDL_Scancode key) {
  return SDL_GetKeyboardState(NULL)[key];
}
} // namespace JJ0o0::TerrainGeneration::Utils