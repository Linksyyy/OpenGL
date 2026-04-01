#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <Core/Shader.hpp>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh {
private:
  GLuint VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  std::vector<float> altVertices;
  GLenum altMode;

public:
  Mesh(const std::vector<Vertex> vertices, const std::vector<GLuint> indices) {
    this->vertices = vertices;
    this->indices = indices;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);

    int stride = sizeof(Vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
  }

  Mesh(const std::vector<float> &vertices, GLenum mode) {
    this->altVertices = vertices;
    this->altMode = mode;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->altVertices.size() * sizeof(float),
                 this->altVertices.data(), GL_STATIC_DRAW);

    int stride = 3 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
  }

  virtual ~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }

  void Draw() const {
    glBindVertexArray(VAO);

    if (!indices.empty()) {
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
      glDrawArrays(altMode, 0, altVertices.size() / 3);
    }
  }
};
