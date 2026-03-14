#pragma once

#include <iostream>
#include "glm/geometric.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {
private:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  float Yaw;
  float Pitch;
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  static constexpr float YAW = -90.0f;
  static constexpr float PITCH = 0.0f;
  static constexpr float SPEED = 2.5f;
  static constexpr float SENSITIVITY = 0.1f;
  static constexpr float ZOOM = 70.0f;

public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
  }

  glm::mat4 GetViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }

  inline void ProcessKeyboard(Camera_Movement direction, float deltaTime,
                              float speedMultiplier = 1.0f) {
    float velocity = MovementSpeed * deltaTime * speedMultiplier;

    if (direction == Camera_Movement::FORWARD)
      Position += glm::normalize(glm::vec3(Front.x, 0.0f, Front.z)) * velocity;
    if (direction == Camera_Movement::BACKWARD)
      Position -= glm::normalize(glm::vec3(Front.x, 0.0f, Front.z)) * velocity;
    if (direction == Camera_Movement::LEFT)
      Position -= Right * velocity;
    if (direction == Camera_Movement::RIGHT)
      Position += Right * velocity;
    if (direction == Camera_Movement::UP)
      Position += WorldUp * velocity;
    if (direction == Camera_Movement::DOWN)
      Position -= WorldUp * velocity;
  }

  inline void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }

    updateCameraVectors();
  }

  inline void ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
      Zoom = 1.0f;
    if (Zoom > 45.0f)
      Zoom = 45.0f;
  }

  inline void logPosition() const {
    std::cout << "\rPosition(" << this->Position.x << ", " << this->Position.y << ", "
              << this->Position.z << ")   "
              << "Pitch: " << this->Pitch << " Yaw: " << this->Yaw << std::flush;
  }

  inline float GetZoom() const { return Zoom; }

private:
  inline void updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};
