#version 460 core

layout (location = 0) in vec3 aPos;

uniform vec2 screenSize;

void main() {
  // Convert from pixel coordinates to NDC (-1 to 1)
  vec2 ndc = (aPos.xy / screenSize) * 2.0;
  gl_Position = vec4(ndc, 0.0, 1.0);
}
