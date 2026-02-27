# Learn OpenGL

This is a project to follow the tutorials from the [learnopengl.com](https://learnopengl.com/) website.

## Prerequisites

*   A C++ compiler (GCC, Clang, MSVC)
*   CMake (version 3.10 or higher)
*   vcpkg

## Dependencies

The following libraries are used:

*   [glad](https://github.com/Dav1dde/glad) - Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
*   [glfw](https://github.com/glfw/glfw) - A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input

## How to Build and Run

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd learn-openGL
    ```

2.  **Configure the project with CMake:**
    ```bash
    cmake -B build
    ```

3.  **Build the project:**
    ```bash
    cmake --build build
    ```

4.  **Run the program:**
    The executable will be generated in the `build/` or `build/Debug/` directory.
    ```bash
    ./build/out
    ```