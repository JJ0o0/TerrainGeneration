#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <TerrainGeneration/graphics/Camera.hpp>
#include <TerrainGeneration/graphics/Shader.hpp>
#include <TerrainGeneration/graphics/Terrain.hpp>
#include <glad/glad.h>

namespace JJ0o0::TerrainGeneration {
class App {
public:
  App(const char *title, int width, int height);
  ~App();

  void run();

private:
  SDL_Window *m_window;
  SDL_GLContext m_context;

  Uint64 m_last;

  int m_width, m_height;
  float m_deltatime;
  bool m_running;

  Graphics::Camera m_camera;
  Graphics::Shader *m_shader;
  Graphics::Terrain *m_terrain;

  void update();
  void render();
  void handleEvents(const SDL_Event &event);
};
} // namespace JJ0o0::TerrainGeneration