#version 460 core

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  vec3 ambient;
  float shininess;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

void main() {
  float dist = distance(light.position, FragPos);
  float attenuation = 1.0 / (1.0 + 0.001 * dist + 0.000002 * (dist * dist));
  attenuation = max(attenuation, 0.20);

  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(normalize(Normal), lightDir), 0.0) * 2.5;

  vec3 viewDir = normalize(cameraPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, normalize(Normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 ambient =
      (light.ambient + material.ambient * 0.30) * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
  specular *= attenuation * 0.18;
  diffuse *= attenuation;

  vec3 result = ambient + diffuse + specular;
  FragColor =  vec4(result, 1.0);
}
