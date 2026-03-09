#ifndef VORONOI_NOISE_H
#define VORONOI_NOISE_H

#include <cmath>
#include <cstdlib>
#include <vector>
#include <limits>

class VoronoiNoise {
public:
  VoronoiNoise(int seed, int gridSize = 10) : gridSize(gridSize) {
    srand(seed);
    generatePoints();
  }

  // x, y esperados em [0, 1]
  float noise(float x, float y) {
    int xCell = (int)floor(x * gridSize);
    int yCell = (int)floor(y * gridSize);

    float minDist = std::numeric_limits<float>::max();
    for (int dy = -2; dy <= 2; dy++) {
      for (int dx = -2; dx <= 2; dx++) {
        int nx = xCell + dx;
        int ny = yCell + dy;

        // wrapping para evitar borda quadrada
        int wx = ((nx % gridSize) + gridSize) % gridSize;
        int wy = ((ny % gridSize) + gridSize) % gridSize;

        // posição real do ponto no espaço [0, 1]
        float px = (nx + points[wy * gridSize + wx].ox) / gridSize;
        float py = (ny + points[wy * gridSize + wx].oy) / gridSize;

        float d = dist(x, y, px, py);
        if (d < minDist)
          minDist = d;
      }
    }
    // normalize to [0, 1]: max possible dist in a cell is sqrt(2)/(2*gridSize)
    float maxDist = sqrtf(2.0f) / (2.0f * gridSize);
    return minDist / maxDist;
  }

private:
  struct Point {
    float ox, oy; // offset dentro da célula, [0, 1]
  };

  int gridSize;
  std::vector<Point> points;

  void generatePoints() {
    points.resize(gridSize * gridSize);
    for (auto &p : points) {
      p.ox = (float)rand() / (float)RAND_MAX;
      p.oy = (float)rand() / (float)RAND_MAX;
    }
  }

  float dist(float x1, float y1, float x2, float y2) {
    float dx = x1 - x2, dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
  }
};

#endif
