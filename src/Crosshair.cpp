#include "../inc/Crosshair.h"

Crosshair::Crosshair(const Shader &shader, const float &length, const glm::vec3 &color, const unsigned int &screenWidth, const unsigned int &screenHeight)
    :shader(shader),
    vertices{
        (screenWidth - length) / 2.0f, screenHeight / 2.0f,
        (screenWidth + length) / 2.0f, screenHeight / 2.0f,
        screenWidth / 2.0f, (screenHeight - length) / 2.0f,
        screenWidth / 2.0f, (screenHeight + length) / 2.0f,
}
{
    this->length = length;
    this->color = color;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

}

Crosshair::~Crosshair()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Crosshair::init()
{
    if (glGetError() == GL_INVALID_OPERATION)
    {
        std::cout << "The OpenGL context was not created";
        throw "The OpenGL context was not created";
    }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), 0.0f, static_cast<GLfloat>(screenHeight));
    shader.use();
    shader.setMat4("projection", projection);
    shader.setVec3("color", color);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Crosshair::renderCrosshair()
{
    shader.use();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 4);

    glUseProgram(NULL);
}
