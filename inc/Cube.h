#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
#include "DirectLight.h"

class Cube
{
private:
    // cube info
    float length_x;
    float length_y;
    float length_z;
    glm::vec3 position;
    glm::vec3 color;

    const Shader &shader;
    float cubeVertices[36 * 6];

    GLuint VBO;
    GLuint VAO;

    const Camera &camera;
    const DirectLight &directLight;
    void initVertice();
    void setMatrix();
public:
    Cube(const glm::vec3 &position, const float &length_x, const float &length_y, const float &length_z, const glm::vec3 &color, const Shader &shader, const Camera &camera, const DirectLight &directLight);
    ~Cube();
    void renderCube();
};