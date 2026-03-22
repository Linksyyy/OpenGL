#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Core/Window.hpp"
#include "Core/Shader.hpp"
#include "Core/Camera.hpp"
#include "Core/stb_image.hpp"
#include "Geometry/Cube.hpp"
#include "Geometry/Mesh.hpp"
#include "Perlin.hpp"

PerlinNoise perlin(90);

float deltaTime{0.0}, lastFrame{0.0f};

Camera camera(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 225.0f, -32.5f);

glm::vec3 lightPos(0, 0, 0);

int VIEW_WIDTH{931}, VIEW_HEIGHT{961};

void processInput(GLFWwindow *window);
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int main() {
  Window window("Bah tche slk", VIEW_WIDTH, VIEW_HEIGHT);
  window.SetCursorPosCallback(mouseCallback);
  window.SetFrameBufferSizeCallback(frameBufferSizeCallback);
  window.SetScrollCallback(scrollCallback);

  float axisOffset = 10000.0f;
  std::vector<float> axisVertices{
      -axisOffset, 0.0f,       0.0f, axisOffset, 0.0f, 0.0f,        0.0f, -axisOffset, 0.0f,
      0.0f,        axisOffset, 0.0f, 0.0f,       0.0f, -axisOffset, 0.0f, 0.0f,        axisOffset,
  };
  Mesh axis(axisVertices, GL_LINES);
  Shader axisShader("./shaders/debug/axis_v.glsl", "./shaders/debug/axis_f.glsl");

  float cursorSize = 15.0f;
  std::vector<float> cursorVertices{
      cursorSize, 0.0f,       0.0f, -cursorSize, 0.0f,        0.0f,
      0.0f,       cursorSize, 0.0f, 0.0f,        -cursorSize, 0.0f,
  };
  Mesh cursor(cursorVertices, GL_LINES);
  Shader cursorShader("./shaders/hud/cursor_v.glsl", "./shaders/hud/cursor_f.glsl");

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("./assets/bricks.png", &width, &height, &nrChannels, 0);
  GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  glClearColor(0.1058, 0.1616, 0.1844, 1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Shader screenShader("./shaders/post/screen_v.glsl", "./shaders/post/screen_f.glsl");
  Shader shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
  Shader lightShader("./shaders/vertex.glsl", "./shaders/light_f.glsl");

  Cube cube;

  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window.GetWindow())) {
    processInput(window.GetWindow());

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()),
                                            (float)VIEW_WIDTH / (float)VIEW_HEIGHT, 0.1f, 4000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model(1.0f);

    glm::vec3 lightColor;
    lightColor.x = sin(glfwGetTime() * 2.0f);
    lightColor.y = sin(glfwGetTime() * 0.7f);
    lightColor.z = sin(glfwGetTime() * 1.3f);
    lightColor *= 2.0f;

    lightShader.Use();
    lightShader.SetVec3("lightColor", lightColor);
    lightShader.SetMat4("projection", projection);
    lightShader.SetMat4("view", view);
    model = glm::translate(model, lightPos);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
    lightShader.SetMat4("model", model);
    glm::vec3 initialLightPos = glm::vec3(0.0f, 9.0f, 10.0f);
    glm::mat4 lightModel(1.0f);
    lightModel = glm::rotate(lightModel, (float)glfwGetTime() * 2.0f, glm::vec3(0, 1, 0));
    lightModel = glm::translate(lightModel, initialLightPos);
    lightPos = glm::vec3(lightModel * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    cube.Draw();

    shader.Use();
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);
    shader.SetVec3("material.ambient", glm::vec3(0.1f));
    shader.SetVec3("material.diffuse", glm::vec3(1.0f));
    shader.SetVec3("material.specular", glm::vec3(0.3f));

    shader.SetVec3("light.position", lightPos);
    shader.SetVec3("light.ambient", lightColor * 0.2f);
    shader.SetVec3("light.diffuse", lightColor * 0.6f);
    shader.SetVec3("light.specular", lightColor);
    shader.SetFloat("material.shininess", 32.0f);
    shader.SetVec3("cameraPos", camera.GetPosition());

    glBindTexture(GL_TEXTURE_2D, texture);
    int worldSize = 127;
    int viewRange = 80;
    glm::vec3 cameraPos = camera.GetPosition();
    for (int i = cameraPos.x - viewRange; i < cameraPos.x + viewRange; i++) {
      for (int j = cameraPos.z - viewRange; j < cameraPos.z + viewRange; j++) {
        int dx = i - cameraPos.x;
        int dz = j - cameraPos.z;
        if (sqrt(dx * dx + dz * dz) > viewRange) {
          continue;
        }

        int noise = perlin.noise((double)i * 0.05f, (double)j * 0.05f) * 3.0f;

        // for (int k = 0; k <= noise + 4; k++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(i, noise, j));
        shader.SetMat4("model", model);
        shader.SetVec3("lightPos", lightPos);
        cube.Draw();
        // }
      }
    }

    axisShader.Use();
    axisShader.SetMat4("projection", projection);
    axisShader.SetMat4("view", view);
    axis.Draw();

    cursorShader.Use();
    cursorShader.SetVec2("screenSize", glm::vec2((float)VIEW_WIDTH, (float)VIEW_HEIGHT));
    cursor.Draw();

    glfwSwapBuffers(window.GetWindow());
    glfwPollEvents();

    camera.logPosition();
  }
  glfwTerminate();
}

void processInput(GLFWwindow *window) {
  float normalSpeed = 4.0f;
  float speedMultiplier = normalSpeed;
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    speedMultiplier *= 3.0f;
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
