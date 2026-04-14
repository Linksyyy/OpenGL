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
uniform float gridMaxRadius;

void main() {
  vec3 worldPos = aPos;
  float deformation = 0.0;

  for(int i = 0; i < bodyCount; i++) {
    vec3 toBody = bodys[i].position - worldPos;
    float r = length(toBody);
    float rs = max(bodys[i].radius, 0.001);
    float rRef = max(gridMaxRadius, 1.0);

    if (rRef > rs) {
      float t = clamp(r / rRef, 0.0, 1.0);

      float broadSheet = rs * (t * t);

      float localWell = (rs * 12.0) / (1.0 + 0.015 * r);
      float localEdge = (rs * 12.0) / (1.0 + 0.015 * rRef);

      deformation += broadSheet + (localWell - localEdge);
    }
  }

  vec3 deformedPos = worldPos;
  deformedPos.y -= deformation;

  gl_Position = projection * view * model * vec4(deformedPos, 1.0f);
}
