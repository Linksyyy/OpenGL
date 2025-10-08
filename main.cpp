#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int INITIAL_WINDOW_WIDTH = 1600;
const int INITIAL_WINDOW_HEIGHT = 800;

const char *vertexShaderSource = R"(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  void main()
  {
     gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)";

const char *fragmentShaderSource = R"(
  #version 330 core
  out vec4 FragColor;

  void main()
  {
      FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
  } 
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Learn OpenGL", NULL, NULL); 
  if(window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    return -1;
  }
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // # shaders
  int sucess;
  char infoLog[512];

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
  if(!sucess) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "=ERROR::SHADER::VERTEX::COMPILATION_FAILED=\n" << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, & fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
  if(!sucess) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "=ERROR::SHADER::FRAGMENT::COMPILATION_FAILED=\n" << infoLog << std::endl;
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
  if(!sucess) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "=ERROR::SHADER::VERTEX::COMPILATION_FAILED=\n" << infoLog << std::endl;
  }

  glUseProgram(shaderProgram);
  
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0); 
  
  // # vertices
  float vertices[] = {
    -0.5, -0.5, 0.0,
    0.5, -0.5, 0.0,
    0.0, 0.5, 0.0
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // # VAO ( Vertex Array Object )
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  
  glBindVertexArray(VAO); 
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // # render loop
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  while(!glfwWindowShouldClose(window)) {
    process_input(window);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    std::cout << "ESC pressed, closing the window" << std::endl;
  }
}
