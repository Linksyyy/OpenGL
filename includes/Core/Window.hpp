#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
private:
  GLFWwindow *window;

public:
  Window(const char *title, const int width, const int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
      std::cout << "Failed to create window" << std::endl;
      return;
    }
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, width, height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  ~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  inline void SetFrameBufferSizeCallback(void frameBufferSizeCallback(GLFWwindow *window, int width,
                                                                      int height)) {
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
  }

  inline void SetCursorPosCallback(void mouseCallback(GLFWwindow *window, double xpos,
                                                      double ypos)) {
    glfwSetCursorPosCallback(window, mouseCallback);
  }

  inline void SetScrollCallback(void scrollCallback(GLFWwindow *window, double xoffset,
                                                    double yoffset)) {
    glfwSetScrollCallback(window, scrollCallback);
  }

  inline GLFWwindow *GetWindow() const { return window; }
};
