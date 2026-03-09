#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/trigonometric.hpp"
#include "stb_image.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Perlin.hpp>
#include <Voronoi.hpp>

char title[] = "Bah tche slk";
float deltaTime{0.0}, lastFrame{0.0f};

Camera camera(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 225.0f, -32.5f);

int VIEW_WIDTH{931}, VIEW_HEIGHT{961};

void processInput(GLFWwindow *window);
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(VIEW_WIDTH, VIEW_HEIGHT, title, NULL, NULL);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glViewport(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  float axisOffset = 1000000.0f;
  float axisVertices[] = {
      -axisOffset, 0.0f,       0.0f, axisOffset, 0.0f, 0.0f,        0.0f, -axisOffset, 0.0f,
      0.0f,        axisOffset, 0.0f, 0.0f,       0.0f, -axisOffset, 0.0f, 0.0f,        axisOffset,
  };

  unsigned int axisVBO, axisVAO;
  glGenVertexArrays(1, &axisVAO);
  glGenBuffers(1, &axisVBO);

  glBindVertexArray(axisVAO);
  glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  int R = 1;
  int precision = 24;
  float factor = 360.0f / precision;
  std::vector<float> vertices = {0.0f, 0.0f, 0.0f};
  for (int i = 0; i <= precision; i++) {
    vertices.push_back(sin(glm::radians((float)i * factor)) * (double)R);
    vertices.push_back(cos(glm::radians((float)i * factor)) * (double)R);
    vertices.push_back(0.0f);
  }
  std::vector<unsigned int> indices;
  for (int i = 0; i <= precision; i++) {
    indices.push_back(0);
    indices.push_back(i);
    indices.push_back(i + 1);
  }

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
               GL_STATIC_DRAW);

  int stride = 3 * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(0);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("assets/grass.png", &width, &height, &nrChannels, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);

  stbi_image_free(data);

  Shader ourShader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  glClearColor(0, 0, 0, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)VIEW_WIDTH / (float)VIEW_HEIGHT, 0.1f, 4000.0f);
    ourShader.setMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);
    ourShader.use();
    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 6);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
}

void processInput(GLFWwindow *window) {
  float speedMultiplier = 1.0;
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    speedMultiplier *= 10.0f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_SPACE))
    camera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    camera.ProcessKeyboard(DOWN, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  VIEW_WIDTH = width;
  VIEW_HEIGHT = height;
  glViewport(0, 0, width, height);
}
bool firstMouse = true;
float lastX, lastY;
void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = (float)xpos;
    lastY = (float)ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}
