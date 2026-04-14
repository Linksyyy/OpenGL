#pragma once

#include "Core/Texture.hpp"
#include "Geometry/Mesh.hpp"
#include <glm/glm.hpp>

class Body {
public:
  static constexpr float RADIUS_SCALE = 2.0e3f;
  static constexpr float MASS_SCALE = 9.0e26f;
  static constexpr float DISTANCE_SCALE = 4.0e4f;

  const Mesh *mesh;
  const Texture *texture;
  glm::vec3 velocity;
  glm::vec3 position;
  const float mass;
  float radius;

  Body(Mesh *mesh, float radius, float mass, glm::vec3 velocity, glm::vec3 position,
       Texture *texture)
      : mesh(mesh), velocity(velocity / DISTANCE_SCALE), position(position / DISTANCE_SCALE),
        mass(mass / MASS_SCALE), radius(radius / RADIUS_SCALE), texture(texture) {}

  void Draw() {
    texture->Use();
    mesh->Draw();
  }

  inline float getMass() { return mass; }

  inline float getRadius() { return radius; }

  inline glm::vec3 getDirection() { return velocity; }
};
