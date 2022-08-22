#include "../inc/camera.h"
#include <iostream>

const glm::vec3 Camera::WORLDUP_DEFAULT = glm::vec3(0.0f, 1.0f, 0.0f);
const float Camera::SPEED_DEFAULT = 2.5f;
const float Camera::SENS_DEFAULT = 0.08f;
const float Camera::FOV_DEFAULT = 90.0f;
const float Camera::ASPECT_DEFAULT = 16.0f / 9.0f;
const float Camera::NEAR_DEFAULT = 0.1f;
const float Camera::FAR_DEFAULT = 100.0f;

Camera::Camera(const glm::vec3 &position, const glm::vec3 &front)
{
    this->position = position;
    this->front = front;
    frontHorizontal = glm::normalize(glm::vec3(front.x, 0, front.z));

    // default initialize
    worldUp = WORLDUP_DEFAULT;

    glm::vec3 x(1.0f, 0.0f, 0.0f);
    yaw = glm::degrees(acos(glm::length(x * frontHorizontal) / glm::length(x) * glm::length(frontHorizontal)));
    if (front.z < 0) yaw = -yaw;
    pitch = glm::degrees(acos(glm::length(front * frontHorizontal) / glm::length(front) * glm::length(frontHorizontal)));
    if (front.y < 0) pitch = -pitch;
    movementSpeed = SPEED_DEFAULT;
    mouseSens = SENS_DEFAULT;
    fov = FOV_DEFAULT;
    aspect = ASPECT_DEFAULT;
    near = NEAR_DEFAULT;
    far = FAR_DEFAULT;

    updateCameraArgs();
}

void Camera::setSpeed(const float &speed)
{
    this->movementSpeed = speed;
}

void Camera::setSensitivity(const float &sens)
{
    this->mouseSens = sens;
}

void Camera::setAspect(const float &aspect)
{
    this->aspect = aspect;
}

glm::vec3 Camera::getPosition() const
{
    return position;
}

glm::vec3 Camera::getFront() const
{
    return front;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getPersMatrix() const
{
    return glm::perspective(glm::radians(fov), aspect, near, far);
}


void Camera::bodyMove(const Movement &direction, const float &deltaTime)
{
    float distance = movementSpeed * deltaTime;
    if (direction == Movement::FORWARD) position += front * distance;
    if (direction == Movement::BACKWARD) position -= front * distance;
    if (direction == Movement::LEFT) position -= right * distance;
    if (direction == Movement::RIGHT) position += right * distance;
    if (direction == Movement::WORLD_UP) position += worldUp * distance;
    if (direction == Movement::WORLD_DOWN) position -= worldUp * distance;
}

void Camera::persMove(float xOffset, float yOffset)
{
    xOffset *= mouseSens;
    yOffset *= mouseSens;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraArgs();
}

void Camera::updateCameraArgs()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    frontHorizontal = glm::normalize(glm::vec3(front.x, 0, front.z));
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
