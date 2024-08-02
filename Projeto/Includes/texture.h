#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../External/glm/glm.hpp"
#include <vector>

// Definindo construtor com referência a dimensões da imagem

class Texture {
public:
    Texture(const char* filename); 
    glm::vec3 get_color(float u, float v) const; // Função get_color que lidará com a renderização da imagem

private:
    int width, height, colorSet;
    std::vector<unsigned char> data;
};

#endif
