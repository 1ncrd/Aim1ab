#pragma once

#include <string>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character {
    GLuint     textureID;  // 字形纹理的ID
    glm::ivec2 size;       // 字形大小
    glm::ivec2 bearing;    // 从基准线到字形左部/顶部的偏移值
    GLuint     advance;    // 原点距下一个字形原点的距离
};

class MyPrinter
{
private:
    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
    unsigned int screenWidth;
    unsigned int screenHeight;
    const Shader &textShader;
public:
    MyPrinter(const std::string &fontPath, const Shader &textShader, const unsigned int &screenWidth, const unsigned int &screenHeight);
    ~MyPrinter();
    void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

