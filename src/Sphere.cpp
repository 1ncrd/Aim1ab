#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../inc/Sphere.h"

Sphere::Sphere(const glm::vec3 &center, const float &radius, const glm::vec3 &color, const Shader &shader, const Camera &camera, const DirectLight &directLight, const int &smoothness/* = 16)*/)
    : camera(camera), directLight(directLight), shader(shader)
{
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->smoothness = smoothness;
    shineness = 8;
    posInGrid = -1; // not in grid
}

Sphere::~Sphere()
{
    // remember to release the memory
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Sphere::init()
{
    if (glGetError() == GL_INVALID_OPERATION) // check OpenGL context
    {
        std::cout << "The OpenGL context was not created";
        throw "The OpenGL context was not created";
    }

    vertices.reserve(smoothness * smoothness * 2 * 3);
    int sidesAmount = smoothness;
    for (int i = 0; i < sidesAmount; i++)
    {
        float verticalAngle = 180.0f * i / sidesAmount;
        float horizonalOffset = (i % 2 == 0) ? 180.0f / sidesAmount : 0;
        for (int j = 0; j < sidesAmount; j++)
        {
            float horizonalAngel = 360.0f * j / sidesAmount + horizonalOffset;
            glm::vec3 vertex_0(1.0f), vertex_1(1.0f), vertex_2(1.0f);
            glm::vec3 reletivePos;
            glm::mat4 trans = glm::mat4(1.0f);

            trans = glm::rotate(trans, glm::radians(horizonalAngel), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, glm::radians(verticalAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            reletivePos = radius * glm::vec3(trans * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            vertex_0 = center + reletivePos;

            trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(horizonalAngel - (180.0f / sidesAmount)), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, glm::radians(verticalAngle + (180.0f / sidesAmount)), glm::vec3(0.0f, 0.0f, 1.0f));
            reletivePos = radius * glm::vec3(trans * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            vertex_1 = center + reletivePos;

            trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(horizonalAngel + (180.0f / sidesAmount)), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, glm::radians(verticalAngle + (180.0f / sidesAmount)), glm::vec3(0.0f, 0.0f, 1.0f));
            reletivePos = radius * glm::vec3(trans * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            vertex_2 = center + reletivePos;
            glm::vec3 normal = glm::normalize(glm::cross(vertex_1 - vertex_0, vertex_2 - vertex_0));

            vertices.insert(vertices.end(), { vertex_0.x, vertex_0.y, vertex_0.z });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { vertex_1.x, vertex_1.y, vertex_1.z });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { vertex_2.x, vertex_2.y, vertex_2.z });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });

            trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(horizonalAngel), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, glm::radians(verticalAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            reletivePos = radius * glm::vec3(trans * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            vertex_0 = center + reletivePos;

            trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(horizonalAngel - (360.0f / sidesAmount)), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, glm::radians(verticalAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            reletivePos = radius * glm::vec3(trans * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            vertex_1 = center + reletivePos;

            trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(horizonalAngel - (180.0f / sidesAmount)), glm::vec3(0.0f, 1.0f, 0.0f));
            trans = glm::rotate(trans, glm::radians(verticalAngle + (180.0f / sidesAmount)), glm::vec3(0.0f, 0.0f, 1.0f));
            reletivePos = radius * glm::vec3(trans * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            vertex_2 = center + reletivePos;
            normal = glm::normalize(glm::cross(vertex_1 - vertex_0, vertex_2 - vertex_0));

            vertices.insert(vertices.end(), { vertex_0.x, vertex_0.y, vertex_0.z });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { vertex_1.x, vertex_1.y, vertex_1.z });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { vertex_2.x, vertex_2.y, vertex_2.z });
            vertices.insert(vertices.end(), { normal.x, normal.y, normal.z });
        }
    }
    shader.use();
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // position attribute
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Sphere::setMatrix()
{
    shader.use();
    // camera
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", camera.getPersMatrix());
    shader.setVec3("cameraPos", camera.getPosition());

    // direct light
    shader.setVec3("directLight.direction", directLight.direction);
    shader.setVec3("directLight.ambient", directLight.ambient);
    shader.setVec3("directLight.diffuse", directLight.diffuse);
    shader.setVec3("directLight.specular", directLight.specular);

    // material
    shader.setFloat("material.shininess", shineness);

    shader.setVec3("aColor", color);

    // ---------------
    glUseProgram(NULL);

}

void Sphere::renderSphere()
{
    if (vertices.empty())
    {
        std::cout << "Vertice is empty, maybe the init was fail" << std::endl;
        return;
    }
    if (vertices.size() % 3 != 0)
    {
        std::cout << "The size of vertice is not a multiple of 3" << std::endl;
        return;
    }

    setMatrix();
    shader.use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
    glUseProgram(NULL);
}

//// the normal will be calculate from normal(cross(vertex_1 - vertex_0, vertex_2 - vertex_0))
//void Sphere::renderTriangle(const glm::vec3 &vertex_0, const glm::vec3 &vertex_1, const glm::vec3 &vertex_2)
//{
//    shader.use();
//
//    glm::vec3 normal = glm::normalize(glm::cross(vertex_1 - vertex_0, vertex_2 - vertex_0));
//    float vertices[] = {
//        // vertex                           // color                    // normal
//        vertex_0.x, vertex_0.y, vertex_0.z, normal.x, normal.y, normal.z,
//        vertex_1.x, vertex_1.y, vertex_1.z, normal.x, normal.y, normal.z,
//        vertex_2.x, vertex_2.y, vertex_2.z, normal.x, normal.y, normal.z,
//    };
//    GLuint VBO, triangleVAO;
//    glGenVertexArrays(1, &triangleVAO);
//    glGenBuffers(1, &VBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // position attribute
//    glBindVertexArray(triangleVAO);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glUseProgram(NULL);
//}

void Sphere::move(const glm::vec3 &newCenter)
{
    float xOffset = newCenter.x - center.x;
    float yOffset = newCenter.y - center.y;
    float zOffset = newCenter.z - center.z;
    for (int i = 0; i < vertices.size(); i += 6)
    {
        vertices[i] += xOffset;
        vertices[i + 1] += yOffset;
        vertices[i + 2] += zOffset;
    }
    center = newCenter;
}

glm::vec3 Sphere::getCenter()
{
    return center;
}

float Sphere::getRadius()
{
    return radius;
}

void Sphere::setGridPos()
{
    static bool posOccupation[25] = { false };
    int i = 0;
    for (i = 0; i < 25; i++)
    {
        if (posOccupation[i] == false) break;
    }
    if (i == 25)
    {
        std::cout << "Sphere::Pos is full";
        throw "Sphere::Pos is full";
    }
    srand(time(0));
    int nextGridPos = rand() % 25;
    while (posOccupation[nextGridPos] == true)
    {
        nextGridPos = rand() % 25;
    }

    if (posInGrid >= 0)
    {
        posOccupation[posInGrid] = false;
    }
    posOccupation[nextGridPos] = true;
    posInGrid = nextGridPos;

    move(glm::vec3(10.0f + 1.5f + (posInGrid % 5) * 3.0f,
        1.5f + (posInGrid / 5) * 3.0f,
        1.0f));
}