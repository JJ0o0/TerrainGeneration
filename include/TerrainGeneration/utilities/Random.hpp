#pragma once

#include <random>

namespace JJ0o0::TerrainGeneration::Utils {
inline float randomFloat(float from, float to) {
  static std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(from, to);

  return dist(rng);
}
} // namespace JJ0o0::TerrainGeneration::Utils