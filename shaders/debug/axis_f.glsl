#version 460 core

out vec4 Color;

in vec3 aPos;

void main() {
  if(aPos.x > 0 || aPos.x < 0) {
    Color = vec4(1, 0, 0, 1);
  } else if(aPos.y > 0 || aPos.y < 0) {
    Color = vec4(0, 1, 0, 1);
  } else if(aPos.z > 0 || aPos.z < 0) {
    Color = vec4(0, 0, 1, 1);
  }
}
