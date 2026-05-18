#pragma once

#include <TerrainGeneration/graphics/Chunk.hpp>
#include <TerrainGeneration/graphics/Shader.hpp>
#include <atomic>
#include <cstdint>
#include <glm/glm.hpp>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>

namespace JJ0o0::TerrainGeneration::Graphics {
class ChunkManager {
public:
  ChunkManager(int renderDistance, int chunkSize, int scale,
               float heightMultiplier, float seedX, float seedZ);
  ~ChunkManager();

  void update(const glm::vec3 &cameraPos);
  void render(Shader &shader);

private:
  std::thread m_workerThread;
  std::mutex m_loadMutex;
  std::mutex m_readyMutex;
  std::queue<std::pair<int, int>> m_loadQueue;
  std::queue<Chunk *> m_readyQueue;
  std::atomic<bool> m_running{true};

  int m_renderDistance, m_chunkSize, m_scale;
  float m_heightMultiplier, m_seedX, m_seedZ;

  std::unordered_map<uint64_t, Chunk *> m_chunks;

  uint64_t hashCoord(int x, int z) const;
  void loadChunk(int x, int z);
  bool tryLoadOneChunk(int camChunkX, int camChunkZ);
  void unloadFarChunks(int camChunkX, int camChunkZ);
  void workerLoop();
};
} // namespace JJ0o0::TerrainGeneration::Graphics