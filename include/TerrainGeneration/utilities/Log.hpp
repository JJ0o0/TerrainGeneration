#pragma once

#include <string>

namespace JJ0o0::TerrainGeneration::Utils {
class Log {
public:
  static void init();

  static void info(const std::string &msg);
  static void warn(const std::string &msg);
  static void error(const std::string &msg);
  static void trace(const std::string &msg);
};
} // namespace JJ0o0::TerrainGeneration::Utils