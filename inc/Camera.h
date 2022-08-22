#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
private:
    // camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 frontHorizontal;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler angles
    float yaw;
    float pitch;
    // camera options
    float movementSpeed;
    float mouseSens; // mouse sensitivity
    float fov; // field of view
    float aspect;
    float near;
    float far;

public:
    // default camera args
    static const glm::vec3 WORLDUP_DEFAULT;
    static const float YAW_DEFAULT;
    static const float PITCH_DEFAULT;
    static const float SPEED_DEFAULT;
    static const float SENS_DEFAULT;
    static const float FOV_DEFAULT;
    static const float ASPECT_DEFAULT;
    static const float NEAR_DEFAULT;
    static const float FAR_DEFAULT;

    enum class Movement {
        FORWARD, BACKWARD, LEFT, RIGHT, WORLD_UP, WORLD_DOWN
    };
    Camera(const glm::vec3 &position = glm::vec3(0.0f), const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f));
    void setSpeed(const float &speed);
    void setSensitivity(const float &sens);
    void setAspect(const float &aspect);
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getPersMatrix() const;
    void bodyMove(const Movement &direction, const float &deltaTime);
    void persMove(float xOffset, float yOffset);

private:
    void updateCameraArgs();
};