#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr auto LONG_LINE = "---------------------------------------------------\n";
class Shader
{
public:
    enum class CompileType {
        VERTEX,
        FRAGMENT,
        PROGRAM,
    };

    std::string vertexPath;
    std::string fragmentPath;
    unsigned int ID;
    bool hasInit;
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();
    void init();
    void use() const;
    void setBool(const std::string &name, const bool &value) const;
    void setInt(const std::string &name, const int &value) const;
    void setFloat(const std::string &name, const float &value) const;
    void setFloatArray(const std::string &firstElementName, const int &index, const float &value) const;

    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setVec3(const std::string &name, const float &x, const float &y, const float &z) const;
    void setVec3Array(const std::string &firstElementName, const int &elementSize, const int &index, const glm::vec3 &vec) const;

    void setVec4(const std::string &name, const glm::vec4 &vec) const;
    void setVec4(const std::string &name, const float &x, const float &y, const float &z, const float &w = 0.0f) const;
    void setMat4(const std::string &name, const glm::mat4 &matrix) const;
private:
    void checkCompileErrors(unsigned int shader, CompileType type);
};