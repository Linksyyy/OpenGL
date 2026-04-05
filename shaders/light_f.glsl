#version 460 core 
out vec4 Color;

in vec2 TexCoords;

uniform vec3 lightColor;

uniform sampler2D aTexture;

void main() {
  Color = texture(aTexture, TexCoords) * vec4(lightColor, 1.0);
}
