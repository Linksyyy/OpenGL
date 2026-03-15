#version 460 core
layout (location = 0) in vec3 Pos;

out vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

void main() {
  gl_Position = projection * view * vec4(Pos, 1.0);
  aPos = Pos;
}
