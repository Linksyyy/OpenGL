#pragma once

#include <iostream>
#include <glad/glad.h>
#include "stb_image.hpp"

class Texture {
private:
  GLuint texture;

public:
  Texture(const char *texture_path) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
    if (data) {
      GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
      glBindTexture(GL_TEXTURE_2D, 0);
    } else {
      std::cout << "ERROR::TEXTURE::DATA_NOT_FOUND" << std::endl;
    }
  }

  ~Texture() { glDeleteTextures(1, &texture); }

  inline void Use(GLenum textureUnit = GL_TEXTURE0) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
  }
};
