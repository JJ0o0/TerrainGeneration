#pragma once

#include <glad/glad.h>
#include <string>

namespace JJ0o0::TerrainGeneration::Graphics {
enum ShaderTypes { VERTEX, FRAGMENT, PROGRAM };

class Shader {
public:
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();

  void bind() const { glUseProgram(m_id); }
  void unbind() const { glUseProgram(0); }

  template <typename T>
  void setUniform(const std::string &name, const T &value) const;

  GLuint getId() const { return m_id; }

private:
  GLuint m_id;

  GLuint compileShader(const char *shaderFile, const ShaderTypes &type);
  GLuint compileProgram(GLuint &vertexShader, GLuint &fragmentShader);
  GLint getUniformLocation(const std::string &name) const;

  bool checkErrors(const ShaderTypes &type, GLuint &shader) const;

  std::string readFile(const std::string &path) const;
};
} // namespace JJ0o0::TerrainGeneration::Graphics