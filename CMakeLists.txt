cmake_minimum_required(VERSION 4.1)

project(learnOpenGL)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(out main.cpp)

target_link_libraries(out PRIVATE
  glfw
  glad::glad
)
