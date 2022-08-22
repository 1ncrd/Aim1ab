#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
#include "DirectLight.h"


class Sphere
{
private:
    // sphere info
    glm::vec3   center;
    float       radius;
    glm::vec3   color;
    float       shineness;

    int posInGrid;

    const Shader &shader;
    std::vector<float> vertices;// vertex.x, y, z, normal.x, y, z
    int smoothness;

    GLuint VBO;
    GLuint VAO;

    const Camera &camera;
    const DirectLight &directLight;
    void setMatrix();
    //void renderTriangle(const glm::vec3 &vertex_0, const glm::vec3 &vertex_1, const glm::vec3 &vertex_2);
public:
    Sphere(const glm::vec3 &center, const float &radius, const glm::vec3 &color, const Shader &shader, const Camera &camera, const DirectLight &directLight, const int &smoothness = 16);
    ~Sphere();
    void init();
    void renderSphere();
    void move(const glm::vec3 &nextCenter);
    glm::vec3 getCenter();
    float getRadius();
    void setGridPos();
};

