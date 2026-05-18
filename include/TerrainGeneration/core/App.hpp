#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <TerrainGeneration/graphics/Camera.hpp>
#include <TerrainGeneration/graphics/ChunkManager.hpp>
#include <TerrainGeneration/graphics/Shader.hpp>
#include <TerrainGeneration/graphics/Terrain.hpp>
#include <TerrainGeneration/graphics/Water.hpp>

#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>

namespace JJ0o0::TerrainGeneration {
class App {
public:
  App(const char *title, int width, int height);
  ~App();

  void run();

private:
  SDL_Window *m_window = nullptr;
  SDL_GLContext m_context;

  Uint64 m_last;

  int m_width, m_height;
  float m_deltatime, m_time;
  bool m_running;

  Graphics::Camera m_camera;
  Graphics::Shader *m_shader = nullptr;
  Graphics::Shader *m_waterShader = nullptr;
  Graphics::ChunkManager *m_chunkManager = nullptr;
  Graphics::Water *m_water = nullptr;

  glm::vec3 m_lightDir, m_lightColor, m_skyColor;

  void update();
  void render();
  void handleEvents(const SDL_Event &event);
  void updateDayNight();
};
} // namespace JJ0o0::TerrainGeneration