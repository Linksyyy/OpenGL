#pragma one

#include <glad/glad.h>
#include <Geometry/Mesh.hpp>
#include <vector>
#include <cmath>

class Sphere : public Mesh {
public:
  Sphere(float ratio, const unsigned int precision)
      : Mesh(CreateVertices(ratio, precision), CreateIndices(precision)) {}

private:
  static std::vector<Vertex> CreateVertices(float ratio, const unsigned int precision) {
    // The amount of polygons is equal to f(x) = x² + 2x + 1 | x = precision
    std::vector<Vertex> vector;
    ratio = std::abs(ratio);
    for (int i = 0; i <= precision; i++) {
      float phi = i * (M_PI / precision);
      for (int j = 0; j <= precision; j++) {
        float theta = j * (2 * M_PI / precision);

        float x = ratio * sin(phi) * cos(theta);
        float y = ratio * cos(phi);
        float z = ratio * sin(phi) * sin(theta);

        float normX = x / ratio;
        float normY = y / ratio;
        float normZ = z / ratio;

        float u = (float)i / precision;
        float v = (float)j / precision;

        vector.push_back({{x, y, z}, {normX, normY, normZ}, {u, v}});
      }
    }
    return vector;
  }

  static std::vector<GLuint> CreateIndices(const unsigned int precision) {
    std::vector<GLuint> vector;

    for (int i = 0; i < precision; i++) {
      int k1 = i * (precision + 1);
      int k2 = k1 + (precision + 1);

      for (int j = 0; j < precision; j++, k1++, k2++) {
        vector.push_back(k1);
        vector.push_back(k2);
        vector.push_back(k1 + 1);

        vector.push_back(k1 + 1);
        vector.push_back(k2);
        vector.push_back(k2 + 1);
      }
    }
    return vector;
  }
};
