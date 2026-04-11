#pragma once

#include "Core/Texture.hpp"
#include "Geometry/Mesh.hpp"
#include <glm/glm.hpp>

class Body {
public:
  const Mesh *mesh;
  const Texture *texture;
  glm::vec3 velocity;
  glm::vec3 position;
  const float mass;
  const float radius;

  Body(Mesh *mesh, float radius, float mass, glm::vec3 velocity, glm::vec3 position,
       Texture *texture)
      : mesh(mesh), radius(radius), mass(mass), velocity(velocity), position(position),
        texture(texture) {}

  void Draw() {
    texture->Use();
    mesh->Draw();
  }

  inline float getMass() { return mass; }

  inline float getRadius() { return radius; }

  inline glm::vec3 getDirection() { return velocity; }
};
