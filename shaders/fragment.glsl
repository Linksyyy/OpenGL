#version 460 core

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

uniform sampler2D ourTexture;

void main() {
  vec3 ambient = light.ambient * material.ambient;
  float dist = distance(light.position, FragPos);

  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(Normal, lightDir), 0.0) * 0.5;
  float attenuation = 1.0 / (1.0 + 0.045 * dist + 0.0075 * (dist * dist));
  vec3 diffuse = diff * light.diffuse * material.diffuse;

  vec3 viewDir = normalize(cameraPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, Normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = material.specular * spec * light.specular * attenuation;

  vec3 result = ambient + (diffuse * attenuation) + specular;
  FragColor = texture(ourTexture, texCoord) * vec4(result, 1.0);
}
