#include <iostream>
#include "../External/glm/glm.hpp"
#include "../External/glm/gtc/matrix_transform.hpp"
#include "./Includes/ray.h"
#include "./Includes/color.h"
#include "./Includes/hitable.h"
#include "./Includes/sphere.h"
#include "./Includes/hitable_list.h"
#include "./Includes/plane.h"
#include "./Includes/camera.h"
#include "./Includes/trianglemesh.h" 
#include <cmath>
#include "float.h"

#define M_PI 3.14159265358979323846  // Define a constante M_PI como o valor de pi

// Define uma cor vermelha, verde e azul normalizada
const color red = glm::vec3(255.99, 0.0, 0.0);
const color green = glm::vec3(0.0, 255.99, 0.0);
const color blue = glm::vec3(0.0, 0.0, 255.99);

// Função para calcular a cor de um raio, dependendo se ele atinge algum objeto no mundo ou não
color ray_color(const ray& r, hitable *world)
{
    hit_record rec;
    if(world->hit(r, 0.0f, FLT_MAX, rec)){  // Testa se o raio atinge algum objeto no mundo
        return rec.cor;  // Retorna a cor do objeto atingido pelo raio
    }

    color backgroundColor = glm::vec3(0.0,0.0,0.0); // Cor preta para o background
    return backgroundColor;  // Retorna a cor de fundo se o raio não atingir nenhum objeto
}

int main() {
    int nx = 500;  // Largura da imagem
    int ny = 500;  // Altura da imagem

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";  // Imprime o cabeçalho do arquivo PPM

    glm::vec3 origin(0.0f, 0.0f, 0.0f);  // Origem da câmera
    glm::vec3 lookingat(0.0f, 0.0f, -1.0f);  // Ponto para onde a câmera está olhando
    glm::vec3 vup(0.0f, 1.0f, 0.0f);  // Vetor de "up" da câmera
    float distance = 0.3f;  // Distância entre a câmera e o plano da imagem

    // Cria uma lista de objetos hitable, incluindo duas esferas, dois planos e duas malhas
    hitable* list[5];

    list[0] = new sphere(glm::vec3(5, 0, -6), 2, red);
    list[1] = new sphere(glm::vec3(5, -2, -6), 2.5, green);
    list[2] = new plane(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1), blue);



    int v_losango = 6;  // Quantidade de vértices na mesh
    int t_losango = 8;  // Quantidade de triângulos na mesh

    // Lista de vértices dos triângulos

    glm::vec3 pontos_losango[v_losango] = {
        glm::vec3(0.0f, 0.0f, 1.0f - 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f - 1.5f), // Right one
        glm::vec3(0.0f, 1.0f, 0.0f - 2.0f), // Top one
        glm::vec3(-1.0f, 0.0f, 0.0f - 2.5f),
        glm::vec3(0.0f, -1.0f, 0.0f - 2.0f),
        glm::vec3(0.0f, 0.0f, -1.0f - 2.0f)
    };

    // Lista com triplas de índices de vértices do losango
    triple vertices_index_losango[t_losango] = {
        triple(0, 1, 2),
        triple(0, 2, 3),
        triple(0, 3, 4),
        triple(0, 4, 1),
        triple(1, 2, 5),
        triple(2, 3, 5),
        triple(3, 4, 5),
        triple(4, 1, 5)  
    };

    tmesh* losango_mesh = new tmesh(v_losango, t_losango, pontos_losango, vertices_index_losango, green + red);
    list[3] = losango_mesh;  // add losango na mesh

    // Segunda mesh é uma pirâmide simples
    int v_piramide = 5;  // Quantidade de vértices na mesh
    int t_piramide = 6;  // Quantidade de triângulos na mesh
    // Lista de vértices dos triângulos
    glm::vec3 pontos_piramide[v_piramide] = {

        glm::vec3(-2.5 , 2, -3),
        glm::vec3(-3 , -1, -4),
        glm::vec3(-1, -1, -4),
        glm::vec3(-1, -1, -2),
        glm::vec3(-3, -1, -2)
    };
    // Lista com triplas de índices de vértices
    triple vertices_index_piramide[t_piramide] = {
        triple(0, 1, 2),
        triple(0, 2, 3),
        triple(0, 3, 4),
        triple(0, 4, 1),
        triple(1, 2, 3),
        triple(1, 3, 4)
    };

    tmesh* triangulos_2 = new tmesh(v_piramide, t_piramide, pontos_piramide, vertices_index_piramide, blue + green);
    list[4] = triangulos_2;  // Adiciona a segunda malha à lista
    
    // Cria o mundo com a lista de objetos
    hitable* world = new hitable_list(list, 5);
    
    camera cam(origin, lookingat, vup, ny, nx, distance);  // Cria uma câmera

    // Loop para gerar a imagem linha por linha
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);  // Coordenada u do pixel normalizada
            float v = float(j) / float(ny);  // Coordenada v do pixel normalizada
            ray r = cam.get_ray(u, v);  // Obtém o raio correspondente ao pixel na câmera
            color pixel_color = ray_color(r, world);  // Calcula a cor do raio
            write_color(std::cout, pixel_color);  // Escreve a cor no arquivo PPM
        }
    }

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso
}
