#include <TerrainGeneration/utilities/Log.hpp>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace JJ0o0::TerrainGeneration::Utils {

void Log::init() {
  std::shared_ptr<spdlog::logger> logger =
      spdlog::stdout_color_mt("TerrainGeneration");

  spdlog::set_default_logger(logger);
  spdlog::set_pattern("[%H:%M:%S] %^[%n] %v%$");

  auto sink = std::dynamic_pointer_cast<spdlog::sinks::stdout_color_sink_mt>(
      logger->sinks()[0]);
#ifdef _WIN32
  sink->set_color(spdlog::level::info, FOREGROUND_GREEN | FOREGROUND_BLUE);
  sink->set_color(spdlog::level::warn, FOREGROUND_RED | FOREGROUND_GREEN);
  sink->set_color(spdlog::level::err, FOREGROUND_RED);
  sink->set_color(spdlog::level::trace,
                  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
  sink->set_color(spdlog::level::info, "\033[36m");
  sink->set_color(spdlog::level::warn, "\033[33m");
  sink->set_color(spdlog::level::err, "\033[31m");
  sink->set_color(spdlog::level::trace, "\033[37m");
#endif
}

void Log::info(const std::string &msg) { spdlog::info(msg); }
void Log::warn(const std::string &msg) { spdlog::warn(msg); }
void Log::error(const std::string &msg) { spdlog::error(msg); }
void Log::trace(const std::string &msg) { spdlog::trace(msg); }
} // namespace JJ0o0::TerrainGeneration::Utils