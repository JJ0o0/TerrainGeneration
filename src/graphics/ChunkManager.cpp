#include <TerrainGeneration/graphics/ChunkManager.hpp>
#include <TerrainGeneration/utilities/Log.hpp>
#include <cmath>

using namespace JJ0o0::TerrainGeneration::Utils;

namespace JJ0o0::TerrainGeneration::Graphics {

ChunkManager::ChunkManager(int renderDistance, int chunkSize, int scale,
                           float heightMultiplier, float seedX, float seedZ)
    : m_renderDistance(renderDistance), m_chunkSize(chunkSize), m_scale(scale),
      m_heightMultiplier(heightMultiplier), m_seedX(seedX), m_seedZ(seedZ) {
  m_workerThread = std::thread(&ChunkManager::workerLoop, this);
}

ChunkManager::~ChunkManager() {
  m_running = false;
  m_workerThread.join(); // espera a thread terminar

  for (auto &[key, chunk] : m_chunks) {
    delete chunk;
  }

  while (!m_readyQueue.empty()) {
    delete m_readyQueue.front();
    m_readyQueue.pop();
  }
}

uint64_t ChunkManager::hashCoord(int x, int z) const {
  return ((uint64_t)(uint32_t)x << 32) | (uint32_t)z;
}

void ChunkManager::loadChunk(int x, int z) {
  uint64_t key = hashCoord(x, z);
  if (m_chunks.find(key) != m_chunks.end())
    return;

  m_chunks[key] = nullptr;

  std::lock_guard<std::mutex> lock(m_loadMutex);
  m_loadQueue.push({x, z});
}

void ChunkManager::unloadFarChunks(int camChunkX, int camChunkZ) {
  std::vector<uint64_t> toRemove;

  for (auto &[key, chunk] : m_chunks) {
    int cx = (int)(key >> 32);
    int cz = (int)(key & 0xFFFFFFFF);

    int dx = cx - camChunkX;
    int dz = cz - camChunkZ;

    if (std::abs(dx) > m_renderDistance + 1 ||
        std::abs(dz) > m_renderDistance + 1) {
      toRemove.push_back(key);
    }
  }

  for (uint64_t key : toRemove) {
    delete m_chunks[key];
    m_chunks.erase(key);
    Log::info("Unloaded chunk");
  }
}

bool ChunkManager::tryLoadOneChunk(int camChunkX, int camChunkZ) {
  for (int z = -m_renderDistance; z <= m_renderDistance; z++) {
    for (int x = -m_renderDistance; x <= m_renderDistance; x++) {
      uint64_t key = hashCoord(camChunkX + x, camChunkZ + z);
      if (m_chunks.find(key) == m_chunks.end()) {
        loadChunk(camChunkX + x, camChunkZ + z);
        return true;
      }
    }
  }
  return false;
}

void ChunkManager::update(const glm::vec3 &cameraPos) {
  int camChunkX = (int)std::floor(cameraPos.x / ((m_chunkSize - 1) * m_scale));
  int camChunkZ = (int)std::floor(cameraPos.z / ((m_chunkSize - 1) * m_scale));

  {
    std::lock_guard<std::mutex> lock(m_readyMutex);
    while (!m_readyQueue.empty()) {
      Chunk *chunk = m_readyQueue.front();
      m_readyQueue.pop();
      chunk->uploadToGPU();
      uint64_t key = hashCoord(chunk->getChunkX(), chunk->getChunkZ());
      m_chunks[key] = chunk;
    }
  }

  tryLoadOneChunk(camChunkX, camChunkZ);
  unloadFarChunks(camChunkX, camChunkZ);
}

void ChunkManager::render(Shader &shader) {
  shader.bind();
  for (auto &[key, chunk] : m_chunks) {
    if (chunk)
      chunk->render(shader);
  }
}

void ChunkManager::workerLoop() {
  while (m_running) {
    std::pair<int, int> coord = {0, 0};
    bool hasWork = false;

    {
      std::lock_guard<std::mutex> lock(m_loadMutex);
      if (!m_loadQueue.empty()) {
        coord = m_loadQueue.front();
        m_loadQueue.pop();
        hasWork = true;
      }
    }

    if (hasWork) {
      Chunk *chunk = new Chunk(coord.first, coord.second, m_chunkSize, m_scale,
                               m_heightMultiplier, m_seedX, m_seedZ);

      std::lock_guard<std::mutex> lock(m_readyMutex);
      m_readyQueue.push(chunk);
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}

} // namespace JJ0o0::TerrainGeneration::Graphics