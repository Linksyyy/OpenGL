#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

uniform sampler2D ourTexture;

void main() {
  float ambientStrength = 0.05;

  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(normalize(Normal), lightDir), 0.0) * 0.5 + 0.5;
  float dist = distance(lightPos, FragPos);
  float attenuation = 30.0 / (dist * dist);
  vec3 diffuse = diff * lightColor;

  vec3 ambient = ambientStrength * lightColor;
  vec3 result = (ambient + (diffuse * lightColor * attenuation)) * objectColor ;
  FragColor = texture(ourTexture, texCoord) * vec4(result, 1.0);
}
