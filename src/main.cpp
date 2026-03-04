#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "stb_image.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

char title[] = "Bah tche slk";

void processInput(GLFWwindow *window);
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(931, 961, title, NULL, NULL);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glViewport(0, 0, 931, 961);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

  float vertices[] = {
      // position     |   //color   | // texCoords
      0.5,  0.5,  0.0, 1.0, 0.0, 0.0, 1, 1, // top right
      -0.5, 0.5,  0.0, 0.0, 1.0, 0.0, 0, 1, // top left
      -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0, 0, // down left
      0.5,  -0.5, 0.0, 0.0, 0.0, 0.0, 1, 0, // down right
  };

  int indices[] = {0, 1, 2, 0, 2, 3};

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  int stride = 8 * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  unsigned int texture[2];
  glGenTextures(2, texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("assets/dirt.png", &width, &height, &nrChannels, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);

  data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_image_free(data);

  Shader ourShader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  glClearColor(0, 0, 0, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  float speed = 0.1;
  float x = 0, y = 0, z = 0;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    processInput(window);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      y += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      y += -speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      x += -speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      x += speed;
    }

    glm::mat4 trans = glm::mat4(1.0);
    trans = glm::scale(trans, glm::vec3(0.2, 0.2, 0.2));
    trans = glm::translate(trans, glm::vec3(x, y, z));
    ourShader.setMat4("transform", trans);

    ourShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
