#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
  unsigned int ID;
  // // 1. 从文件路径中获取顶点/片段着色器
  // ------------------------------------------------------------------------
  Shader(const char *vertexPath, const char *fragmentPath)
  {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
      // 打开文件
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // 读取文件的缓冲内容到数据流中
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // 关闭文件处理器
      vShaderFile.close();
      fShaderFile.close();
      // 转换数据流到string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. 编译着色器
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  // activate the shader
  // ------------------------------------------------------------------------
  void use()
  {
    glUseProgram(ID);
  }
  // utility uniform functions
  // ------------------------------------------------------------------------
  void setBool(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  // -------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const
  {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(unsigned int shader, std::string type)
  {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
    else
    {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
};
#endif