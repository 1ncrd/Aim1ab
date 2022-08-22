#include "../inc/Cube.h"

Cube::Cube(const glm::vec3 &position, const float &length_x, const float &length_y, const float &length_z, const glm::vec3 &color, const Shader &shader, const Camera &camera, const DirectLight &directLight)
    : camera(camera), directLight(directLight), shader(shader)
{
    this->position = position;
    this->length_x = length_x;
    this->length_y = length_y;
    this->length_z = length_z;
    this->color = color;
    initVertice();
}

Cube::~Cube()
{
    // remember to release the memory
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Cube::initVertice()
{
    float vertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    };
    for (int i = 0; i < 36 * 6; i++)
    {
        cubeVertices[i] = vertices[i];
    }

    for (int i = 0; i < 36 * 6; i += 6)
    {
        cubeVertices[i] += 0.5f;
        cubeVertices[i + 1] += 0.5f;
        cubeVertices[i + 2] += 0.5f;
        if (cubeVertices[i] > 0.01f) cubeVertices[i] *= length_x;
        if (cubeVertices[i + 1] > 0.01f) cubeVertices[i + 1] *= length_y;
        if (cubeVertices[i + 2] > 0.01f) cubeVertices[i + 2] *= length_z;
    }

    for (int i = 0; i < 36 * 6; i += 6)
    {
        cubeVertices[i] += position.x;
        cubeVertices[i + 1] += position.y;
        cubeVertices[i + 2] += position.z;
    }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Cube::renderCube()
{
    setMatrix();
    shader.use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUseProgram(NULL);
}

void Cube::setMatrix()
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
    shader.setFloat("material.shininess", 16);

    shader.setVec3("aColor", color);

    // ---------------
    glUseProgram(NULL);

}