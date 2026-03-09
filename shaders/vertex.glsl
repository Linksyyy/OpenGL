
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float time;

void main() {
  vec3 pos = aPos;

  gl_Position = projection * view * model * vec4(pos, 1.0);
  texCoord = aTexCoord;
}
