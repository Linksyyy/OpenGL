#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <cmath>
#include <cstdlib>

class PerlinNoise {
public:
  PerlinNoise(int seed) {
    srand(seed);
    for (int i = 0; i < 256; i++) {
      p[i] = i;
    }
    for (int i = 0; i < 256; i++) {
      int j = rand() % 256;
      int temp = p[i];
      p[i] = p[j];
      p[j] = temp;
    }
    for (int i = 256; i < 512; i++) {
      p[i] = p[i - 256];
    }
  }

  float noise(float x, float y) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    x -= floor(x);
    y -= floor(y);
    float u = fade(x);
    float v = fade(y);
    int A = p[X] + Y;
    int B = p[X + 1] + Y;
    return lerp(v, lerp(u, grad(p[A], x, y), grad(p[B], x - 1, y)),
                lerp(u, grad(p[A + 1], x, y - 1), grad(p[B + 1], x - 1, y - 1)));
  }

private:
  int p[512];

  float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

  float lerp(float t, float a, float b) { return a + t * (b - a); }

  float grad(int hash, float x, float y) {
    int h = hash & 3;
    float u = h < 2 ? x : -x;
    float v = h & 1 ? y : -y;
    return u + v;
  }
};

#endif
