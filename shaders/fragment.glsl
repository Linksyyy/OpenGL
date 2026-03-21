#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform sampler2D ourTexture;

void main() {
  float ambientStrength = 0.05;
  float specularStrength = 0.05;

  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(Normal, lightDir), 0.0) * 0.5 + ambientStrength;
  float dist = distance(lightPos, FragPos);
  float attenuation = 1.0 / (1.0 + 0.045 * dist + 0.0075 * (dist * dist));
  vec3 diffuse = diff * lightColor;

  vec3 viewDir = normalize(cameraPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, Normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
  vec3 specular = specularStrength * spec * lightColor; 

  vec3 ambient = ambientStrength * lightColor;
  vec3 result = (ambient + (diffuse * lightColor * attenuation) + specular) * objectColor ;
  FragColor = texture(ourTexture, texCoord) * vec4(result, 1.0);
}
