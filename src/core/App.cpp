#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

#include <TerrainGeneration/core/App.hpp>
#include <TerrainGeneration/utilities/Colors.hpp>
#include <TerrainGeneration/utilities/Input.hpp>
#include <TerrainGeneration/utilities/Log.hpp>
#include <TerrainGeneration/utilities/Random.hpp>
#include <TerrainGeneration/utilities/UI.hpp>
#include <glm/trigonometric.hpp>

namespace JJ0o0::TerrainGeneration {
App::App(const char *title, int width, int height)
    : m_width(width), m_height(height), m_running(true), m_time(0.0f) {
  SDL_Init(SDL_INIT_VIDEO);
  Utils::Log::init();

  Utils::Log::info("Welcome to my Terrain Generation program!");
  Utils::Log::info("Loading...");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
  m_context = SDL_GL_CreateContext(m_window);
  SDL_GL_SetSwapInterval(1);
  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
  glEnable(GL_DEPTH_TEST);

  m_shader = new Graphics::Shader("assets/shaders/terrain.vert",
                                  "assets/shaders/terrain.frag");
  m_waterShader = new Graphics::Shader("assets/shaders/water.vert",
                                       "assets/shaders/water.frag");

  m_chunkManager = new Graphics::ChunkManager(
      4, Graphics::CHUNK_SIZE, 1, 150.0f, Utils::randomFloat(0.0f, 10000.0f),
      Utils::randomFloat(0.0f, 10000.0f));

  m_water = new Graphics::Water(5000.0f, -1.0f);

  m_camera = {{0.0f, 80.0f, 80.0f},
              {0.0f, -0.7f, -0.7f},
              {0.0f, 1.0f, 0.0f},
              glm::radians(-90.0f),
              glm::radians(-45.0f),
              60.0f,
              (float)width / height,
              0.1f,
              2000.0f};

  SDL_SetWindowRelativeMouseMode(m_window, true);

  Utils::Log::info("Loaded!");
}

App::~App() {
  Utils::Log::info("Exiting...");

  if (m_shader) {
    delete m_shader;
    m_shader = nullptr;
  }

  if (m_waterShader) {
    delete m_waterShader;
    m_waterShader = nullptr;
  }

  if (m_chunkManager) {
    delete m_chunkManager;
    m_chunkManager = nullptr;
  }

  if (m_water) {
    delete m_water;
    m_water = nullptr;
  }

  SDL_DestroyWindow(m_window);
  SDL_GL_DestroyContext(m_context);
  SDL_Quit();
}

void App::run() {
  SDL_FlushEvent(SDL_EVENT_MOUSE_MOTION);
  SDL_Event event;

  m_last = SDL_GetTicks();
  while (m_running) {
    while (SDL_PollEvent(&event)) {
      handleEvents(event);
    }

    update();

    glClearColor(m_skyColor.r, m_skyColor.g, m_skyColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render();

    SDL_GL_SwapWindow(m_window);
  }
}

void App::update() {
  Uint64 now = SDL_GetTicks();
  m_deltatime = (now - m_last) / 1000.0f;
  m_last = now;

  m_time += m_deltatime * 0.01;
  if (m_time > 1.0f) {
    m_time -= 1.0f;
  }

  updateDayNight();

  float speed = Utils::isKeyPressed(SDL_SCANCODE_LSHIFT) ? 120.0f : 30.0f;
  glm::vec3 right = glm::normalize(glm::cross(m_camera.front, m_camera.up));

  if (Utils::isKeyPressed(SDL_SCANCODE_W))
    m_camera.position += speed * m_camera.front * m_deltatime;
  if (Utils::isKeyPressed(SDL_SCANCODE_S))
    m_camera.position -= speed * m_camera.front * m_deltatime;
  if (Utils::isKeyPressed(SDL_SCANCODE_D))
    m_camera.position += speed * right * m_deltatime;
  if (Utils::isKeyPressed(SDL_SCANCODE_A))
    m_camera.position -= speed * right * m_deltatime;
  if (Utils::isKeyPressed(SDL_SCANCODE_SPACE))
    m_camera.position += speed * m_camera.up * m_deltatime;
  if (Utils::isKeyPressed(SDL_SCANCODE_LCTRL))
    m_camera.position -= speed * m_camera.up * m_deltatime;

  m_chunkManager->update(m_camera.position);
}

void App::render() {
  glm::mat4 model = glm::mat4(1.0f);

  m_shader->bind();
  m_shader->setUniform("model", model);
  m_shader->setUniform("view", m_camera.getView());
  m_shader->setUniform("projection", m_camera.getProjection());
  m_shader->setUniform("lightDir", m_lightDir);
  m_shader->setUniform("lightColor", m_lightColor);
  m_shader->setUniform("fogColor", m_skyColor);
  m_shader->setUniform("cameraPos", m_camera.position);
  m_shader->setUniform("fogStart", 50.0f);
  m_shader->setUniform("fogEnd", 250.0f);

  m_chunkManager->render(*m_shader);
  m_shader->unbind();

  m_waterShader->bind();
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glm::mat4 waterModel =
      glm::translate(glm::mat4(1.0f),
                     glm::vec3(m_camera.position.x, 0.0f, m_camera.position.z));

  m_waterShader->setUniform("model", waterModel);
  m_waterShader->setUniform("view", m_camera.getView());
  m_waterShader->setUniform("projection", m_camera.getProjection());
  m_waterShader->setUniform("lightDir", m_lightDir);
  m_waterShader->setUniform("lightColor", m_lightColor);
  m_waterShader->setUniform("cameraPos", m_camera.position);
  m_waterShader->setUniform("fogColor", m_skyColor);
  m_waterShader->setUniform("fogStart", 50.0f);
  m_waterShader->setUniform("fogEnd", 250.0f);
  m_waterShader->setUniform("time", (float)SDL_GetTicks() / 1000.0f);

  m_water->render(*m_waterShader);

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  m_waterShader->unbind();
}

void App::handleEvents(const SDL_Event &event) {
  switch (event.type) {
  case SDL_EVENT_QUIT:
    m_running = false;
    break;
  case SDL_EVENT_KEY_DOWN:
    if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
      if (SDL_GetWindowRelativeMouseMode(m_window)) {
        SDL_SetWindowRelativeMouseMode(m_window, false);
        return;
      }

      m_running = false;
    }

    break;
  case SDL_EVENT_MOUSE_MOTION: {
    if (!SDL_GetWindowRelativeMouseMode(m_window))
      return;

    float sensitivity = 5.0f / 10000.0f;

    m_camera.yaw += event.motion.xrel * sensitivity;
    m_camera.pitch -= event.motion.yrel * sensitivity;

    m_camera.pitch =
        std::clamp(m_camera.pitch, glm::radians(-89.0f), glm::radians(89.0f));

    m_camera.front.x = glm::cos(m_camera.yaw) * glm::cos(m_camera.pitch);
    m_camera.front.y = glm::sin(m_camera.pitch);
    m_camera.front.z = glm::sin(m_camera.yaw) * glm::cos(m_camera.pitch);
    m_camera.front = glm::normalize(m_camera.front);
  }
  }
}

void App::updateDayNight() {
  float angle = m_time * 2.0f * 3.14159f;

  m_lightDir =
      glm::normalize(glm::vec3(0.0f, -std::sin(angle), -std::cos(angle)));

  float sunHeight = -m_lightDir.y;

  if (sunHeight > 0.2f) {
    m_skyColor = Utils::SKY_DAY;
    m_lightColor = Utils::LIGHT_DAY;
  } else if (sunHeight > -0.1f) {
    float t = (sunHeight + 0.1f) / 0.3f;
    m_skyColor = glm::mix(Utils::SKY_SUNSET, Utils::SKY_DAY, t);
    m_lightColor = glm::mix(Utils::LIGHT_SUNSET, Utils::LIGHT_DAY, t);
  } else {
    float t = std::clamp((-sunHeight - 0.1f) / 0.3f, 0.0f, 1.0f);
    m_skyColor = glm::mix(Utils::SKY_SUNSET, Utils::SKY_NIGHT, t);
    m_lightColor = glm::mix(Utils::LIGHT_SUNSET, Utils::LIGHT_NIGHT, t);
  }
}

} // namespace JJ0o0::TerrainGeneration