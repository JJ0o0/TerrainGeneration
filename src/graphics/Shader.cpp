#include <TerrainGeneration/graphics/Shader.hpp>
#include <TerrainGeneration/utilities/Log.hpp>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

using namespace JJ0o0::TerrainGeneration::Utils;

namespace JJ0o0::TerrainGeneration::Graphics {
Shader::Shader(const std::string &vertexFilePath,
               const std::string &fragmentFilePath) {
  std::string vertexFile = readFile(vertexFilePath);
  std::string fragmentFile = readFile(fragmentFilePath);

  GLuint vertexShader = compileShader(vertexFile.c_str(), VERTEX);
  GLuint fragmentShader = compileShader(fragmentFile.c_str(), FRAGMENT);

  m_id = compileProgram(vertexShader, fragmentShader);
}

Shader::~Shader() {
  unbind();
  glDeleteProgram(m_id);
}

GLuint Shader::compileShader(const char *shaderFile, const ShaderTypes &type) {
  if (type == PROGRAM) {
    Log::warn("Use \"compileProgram\" for Program Compilation.");
    return 0;
  }

  GLenum shaderType;
  switch (type) {
  case VERTEX:
    shaderType = GL_VERTEX_SHADER;
    break;
  case FRAGMENT:
    shaderType = GL_FRAGMENT_SHADER;
    break;
  default:
    return 0;
  }

  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderFile, nullptr);
  glCompileShader(shader);

  if (!checkErrors(type, shader)) {
    return 0;
  }

  return shader;
}

GLuint Shader::compileProgram(GLuint &vertexShader, GLuint &fragmentShader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  if (!checkErrors(PROGRAM, program)) {
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

GLint Shader::getUniformLocation(const std::string &name) const {
  GLint location = glGetUniformLocation(m_id, name.c_str());

  if (location == -1) {
    Log::error("Could not find \"" + name + "\" uniform on Shader!");
    return -1;
  }

  return location;
}

bool Shader::checkErrors(const ShaderTypes &type, GLuint &shader) const {
  GLint success;
  char infoLog[512];

  if (type != PROGRAM) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      Log::error("Shader Error: " + std::string(infoLog));
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(shader, 512, nullptr, infoLog);
      Log::error("Program Linking Error: " + std::string(infoLog));
    }
  }

  return success;
}

template <>
void Shader::setUniform(const std::string &name, const int &value) const {
  glUniform1i(getUniformLocation(name), value);
}

template <>
void Shader::setUniform(const std::string &name, const float &value) const {
  glUniform1f(getUniformLocation(name), value);
}

template <>
void Shader::setUniform(const std::string &name, const bool &value) const {
  glUniform1i(getUniformLocation(name), (int)value);
}

template <>
void Shader::setUniform(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(value));
}

template <>
void Shader::setUniform(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

std::string Shader::readFile(const std::string &filepath) const {
  std::ifstream file(filepath);

  if (!file.is_open()) {
    Log::error("Failed to open file: " + filepath);
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
} // namespace JJ0o0::TerrainGeneration::Graphics