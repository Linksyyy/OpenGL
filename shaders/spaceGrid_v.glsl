#version 460 core
layout (location = 0) in vec3 aPos;

struct celestialBody {
  vec3 position;
  float radius;
  float mass;

};

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform celestialBody bodys[10];
uniform int bodyCount;

void main() {
  vec3 worldPos = aPos;
  float totalInfluence = 0.0f;

  for(int i = 0; i < bodyCount; i++) {
    vec3 toBody = bodys[i].position - worldPos;
    float distance = length(toBody);

    float softDistance = max(distance, 0.1);
    float bodyScale = max(bodys[i].radius, 1.0);
    float mass = max(bodys[i].mass, 0.001);

    float broadSoftening = 20.0 * bodyScale;
    float broadField = mass / (broadSoftening + softDistance * 3.2 + softDistance * softDistance * 0.15);

    float localSoftening = 2.5 * bodyScale;
    float localField = (sqrt(mass) * bodyScale) /
                      (softDistance * softDistance + localSoftening * localSoftening);

    totalInfluence += broadField * 0.50 + localField * 26.0;
  }

  float deformation = sqrt(max(totalInfluence, 0.0)) * 3.9;
  vec3 deformedPos = worldPos;
  deformedPos.y -= deformation;

  gl_Position = projection * view * model * vec4(deformedPos, 1.0f);
}
