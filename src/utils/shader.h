
#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(){ }
    // activate the shader
    // ------------------------------------------------------------------------
    Shader &Use();
    // generates the shader on the fly
    // ------------------------------------------------------------------------
    void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // Compile the shader program
    // utility uniform functions
    // ------------------------------------------------------------------------
    void SetBool(const std::string &name, bool value, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetInt(const std::string &name, int value, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetFloat(const std::string &name, float value, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetVec1(const std::string &name, const float *value, unsigned int count, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetVec2(const std::string &name, const glm::vec2 &value, bool useShader = false);
    void SetVec2(const std::string &name, float x, float y, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetVec3(const std::string &name, const glm::vec3 &value, bool useShader = false);
    void SetVec3(const std::string &name, float x, float y, float z, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetVec4(const std::string &name, const glm::vec4 &value, bool useShader = false);
    void SetVec4(const std::string &name, float x, float y, float z, float w, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetMat2(const std::string &name, const glm::mat2 &mat, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetMat3(const std::string &name, const glm::mat3 &mat, bool useShader = false);
    // ------------------------------------------------------------------------
    void SetMat4(const std::string &name, const glm::mat4 &mat, bool useShader = false);

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(GLuint shader, std::string type);

};
#endif

