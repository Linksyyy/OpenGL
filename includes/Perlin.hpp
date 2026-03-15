#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

class PerlinNoise {
private:
  int p[512];

  double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

  double lerp(double t, double a, double b) { return a + t * (b - a); }

  double grad(int hash, double x, double y) {
    // Converte os 4 bits inferiores do hash em 8 direções de gradiente
    int h = hash & 7;
    double u = h < 4 ? x : y;
    double v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
  }

public:
  PerlinNoise(unsigned int seed = std::default_random_engine::default_seed) {
    for (int i = 0; i < 256; i++)
      p[i] = i;

    std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

    for (int i = 0; i < 256; i++)
      p[256 + i] = p[i];
  }

  double noise(double x, double y) {
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    double u = fade(x);
    double v = fade(y);

    int aa = p[p[X] + Y];
    int ab = p[p[X] + Y + 1];
    int ba = p[p[X + 1] + Y];
    int bb = p[p[X + 1] + Y + 1];

    return lerp(v, lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
                lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1)));
  }
};
