#include "Geometry/Cube.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Core/Shader.hpp>
#include <Core/Camera.hpp>
#include <Core/stb_image.hpp>

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

  float axisOffset = 10000.0f;
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

  Shader axisShader("./shaders/debug/axis_v.glsl", "./shaders/debug/axis_f.glsl");

  unsigned int texture;
  glGenTextures(1, &texture);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("./assets/dirt.png", &width, &height, &nrChannels, 0);
  GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  Shader ourShader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  // glClearColor(0.51f, 0.79f, 1.0f, 1.0f);
  glClearColor(0, 0, 0, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Cube cube;

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()),
                                            (float)VIEW_WIDTH / (float)VIEW_HEIGHT, 0.1f, 4000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    cube.Draw(ourShader);
    ourShader.SetMat4("projection", projection);
    ourShader.SetMat4("view", view);
    ourShader.SetMat4("model", model);

    axisShader.Use();
    axisShader.SetMat4("projection", projection);
    axisShader.SetMat4("view", view);

    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();

    camera.logPosition();
  }
  glfwTerminate();
}

void processInput(GLFWwindow *window) {
  float normalSpeed = 4.0f;
  float speedMultiplier = normalSpeed;
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    speedMultiplier *= 5.0f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime, speedMultiplier);
  if (glfwGetKey(window, GLFW_KEY_SPACE))
    camera.ProcessKeyboard(Camera_Movement::UP, deltaTime, normalSpeed);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime, normalSpeed);
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
