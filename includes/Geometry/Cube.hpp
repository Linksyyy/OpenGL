#pragma once

#include <Geometry/Mesh.hpp>
#include <vector>

class Cube : public Mesh {
public:
  Cube(float size = 1.0f) : Mesh(CreateVertices(size), CreateIndices()) {}

private:
  static std::vector<Vertex> CreateVertices(float size) {
    float s = size / 2.0f;
    return {
        {{-s, -s, s}, {0, 0, 1}, {0, 0}},
        {{s, -s, s}, {0, 0, 1}, {1, 0}},
        {{s, s, s}, {0, 0, 1}, {1, 1}},
        {{-s, s, s}, {0, 0, 1}, {0, 1}},

        {{s, -s, -s}, {0, 0, -1}, {0, 0}},
        {{-s, -s, -s}, {0, 0, -1}, {1, 0}},
        {{-s, s, -s}, {0, 0, -1}, {1, 1}},
        {{s, s, -s}, {0, 0, -1}, {0, 1}},

        {{-s, s, -s}, {0, 1, 0}, {0, 0}},
        {{s, s, -s}, {0, 1, 0}, {1, 0}},
        {{s, s, s}, {0, 1, 0}, {1, 1}},
        {{-s, s, s}, {0, 1, 0}, {0, 1}},

        {{-s, -s, -s}, {0, -1, 0}, {0, 0}},
        {{s, -s, -s}, {0, -1, 0}, {1, 0}},
        {{s, -s, s}, {0, -1, 0}, {1, 1}},
        {{-s, -s, s}, {0, -1, 0}, {0, 1}},

        {{s, -s, -s}, {1, 0, 0}, {0, 0}},
        {{s, -s, s}, {1, 0, 0}, {1, 0}},
        {{s, s, s}, {1, 0, 0}, {1, 1}},
        {{s, s, -s}, {1, 0, 0}, {0, 1}},

        {{-s, -s, s}, {-1, 0, 0}, {0, 0}},
        {{-s, -s, -s}, {-1, 0, 0}, {1, 0}},
        {{-s, s, -s}, {-1, 0, 0}, {1, 1}},
        {{-s, s, s}, {-1, 0, 0}, {0, 1}},
    };
  }

  static std::vector<unsigned int> CreateIndices() {
    return {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
    };
  }
};
