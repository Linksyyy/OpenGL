#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Celestial/Body.hpp"

class Gravity {
private:
  // Physical gravitational constant in km^3 / (kg * s^2),
  // converted to simulation units by the same scale factors used in Body.
  static constexpr float G_KM = 6.67430e-20f;
  static constexpr float G = G_KM * (Body::MASS_SCALE /
                                     (Body::DISTANCE_SCALE * Body::DISTANCE_SCALE *
                                      Body::DISTANCE_SCALE));

public:
  static glm::vec3 CalculateForce(const Body &body1, const Body &body2) {
    glm::vec3 direction = body2.position - body1.position;
    float distance = glm::length(direction);

    if (distance < 0.01f)
      return glm::vec3(0);

    float gravitationalForce = G * (body1.mass * body2.mass) / (distance * distance);
    return glm::normalize(direction) * gravitationalForce;
  }

  static glm::vec3 CalculateAcceleration(const Body &target, const std::vector<Body *> &bodies) {
    glm::vec3 netForce(0);

    for (const auto body : bodies) {
      if (body == &target)
        continue;
      netForce += CalculateForce(target, *body);
    }
    return netForce / target.mass;
  }

  static void UpdateVelocity(Body &body, const glm::vec3 &acceleration, float deltaTime) {
    body.velocity += acceleration * deltaTime;
  }

  static void UpdatePosition(Body &body, float deltaTime) {
    body.position += body.velocity * deltaTime;
  }
};
