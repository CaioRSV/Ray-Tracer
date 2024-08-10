#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // https://github.com/nothings/stb/blob/master/stb_image.h
#include <iostream>

// Função de carregamento da imagem com stb_image.h
Texture::Texture(const char* filename) {
    // Aloca na memória e retorna ponteiro pro address
    unsigned char* image = stbi_load(filename, &width, &height, &colorSet, 0);
    if (image) {
        data = std::vector<unsigned char>(image, image + width * height * colorSet);
        stbi_image_free(image);
    } else {
        std::cerr << "Falha ao carregar imagem: " << filename << std::endl;
    }
}

glm::vec3 Texture::get_color(float u, float v) const {
    if (data.empty()) return glm::vec3(1.0f, 0.0f, 1.0f); // Bota uma cor meio roxa caso não tenha textura (falha de carregamento)

    // Encontrando pixel em X e Y baseado no 'u' e 'v'
    // Se baseando nas dimensões e permitindo repetição com o uso do módulo

    u = u - floor(u);
    v = v - floor(v);

    int x = static_cast<int>(u * width) % width; // Módulo para casos negativos principalmente
    int y = static_cast<int>((v) * height) % height; // Pode se inverter a ordem (1-v) pra começar a 'printar' de baixo pra cima

    // Pegando valores RGB referente ao pixel onde está
    int index = (y * width + x) * colorSet; // Encontra o ponto na array do pixel escolhido, e com isso o item das cores
    
    // Selecionando as cores e normalizando
    float r = data[index] / 255.0f; 
    float g = data[index + 1] / 255.0f;
    float b = data[index + 2] / 255.0f;

    return glm::vec3(r, g, b);
}
