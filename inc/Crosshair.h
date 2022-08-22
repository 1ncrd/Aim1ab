#pragma once

#include <glm/glm.hpp>

#include "Shader.h"


class Crosshair
{
private:
    float length;
    glm::vec3 color;
    const Shader &shader;
    unsigned int screenWidth;
    unsigned int screenHeight;
    float vertices[8];

    GLuint VBO;
    GLuint VAO;

public:
    Crosshair(const Shader &shader, const float &length, const glm::vec3 &color, const unsigned int &screenWidth, const unsigned int &screenHeight);
    ~Crosshair();
    void init();
    void renderCrosshair();
};