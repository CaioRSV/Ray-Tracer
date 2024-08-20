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
#include "./Includes/transform.h"
#include "./Includes/material.h"
#include "./Includes/environment.h"
#include "./Includes/light.h"
#include <cmath>
#include "float.h"
#define M_PI 3.14159265358979323846  // Define a constante M_PI como o valor de pi


#include <vector>
#include "./Includes/triangle.h" // Inclua a classe triangle

#include <fstream>
std::ofstream bspLogFile("bsp_tree_log.txt");

using std::vector;

// Materiais básicos de teste
// "matte" significa fosco e sem brilho
// glossy, com muito brilho
//                               d     a     s     r     t     n 
material* matte = new material(0.8f, 0.4f, 0.1f, 0.0f, 0.1f, 1.0f);
material* glossy = new material(0.9f, 0.4f, 0.9f, 0.8f, 1.0f, 50.0f);

material* glass = new material(0.1f, 0.2f, 0.05f, 0.5f, 1.0f, 1.0f);

material* mirror = new material(0.01f, 0.1f, 0.5f, 1.0f, 0.1f, 10.0f);


material* mattePlane = new material(0.2f, 0.4f, 0.1f, 0.0f, 0.0f, 1.0f);
material* glossyPlane = new material(0.8f, 0.4f, 0.5f, 0.3f, 0.0f, 30.0f);


// Luzes de cena
// Luz ambiente branca e pontos de luz local
color white = color(1,1,1);


Environment* ambientLight = new Environment(color(0.1f, 0.1f, 0.1f));

Light* light_point1 = new Light(glm::vec3(0,1,4),white);
Light* light_point2 = new Light(glm::vec3(16,1,-16),white);

Light* light_point3 = new Light(glm::vec3(-1,-1,6),white);




vector<Light*> scene_lights;

// Define uma cor vermelha, verde e azul normalizada
const color red = glm::vec3(255.99, 0.0, 0.0);
const color green = glm::vec3(0.0, 255.99, 0.0);
const color blue = glm::vec3(0.0, 0.0, 255.99);

const color marineBlue = glm::vec3(0.0, 0.0, 80);



const color whiteMaterial = glm::vec3(255.99, 255.99, 255.99);
const color slate = glm::vec3(0.5, 0.5, 0.5);
const color black = glm::vec3(0.01, 0.01, 0.01);



vec3 phong(hit_record rec, color amb_light, vector<Light*> point_lights, vec3 viewer_pos){

    // Parte ambiental da iluminação de Phong
    vec3 ambient_factor = rec.kamb * amb_light;

    // Este 'sum' será a soma dos componentes difuso e especular para cada luz da cena.
    vec3 sum = vec3(0.0f, 0.0f, 0.0f);
    for(Light* cur_light : point_lights){

        // Vetor normalizado que vai do ponto de interseção em direção à posição da luz
        vec3 L = normalize(cur_light->getPosition() - rec.p);

        // O produto (N . L) da equação de Phong, clamped para não ser negativo
        float diffuse_dot = glm::dot(rec.normal, L);
        diffuse_dot = glm::clamp(diffuse_dot, 0.0f, 1.0f);

        // Parte difusa da iluminação de Phong
        vec3 diffuse_factor = cur_light->getIntensity() * rec.cor * rec.kdif * diffuse_dot;

        // Vetor V vai do ponto de interseção até a posição do observador (câmera)
        vec3 V = normalize(viewer_pos - rec.p);

        // Vetor R é o vetor de reflexão, sendo 2.N (N. L) - L 
        vec3 R = 2.0f * (rec.normal * glm::dot(rec.normal, L)) - L; 

        // O produto (R . V) da equação de Phong
        float specular_dot = glm::dot(R, V);
        specular_dot = glm::clamp(specular_dot, 0.0f, 1.0f);

        // A reflexão elevada à potência do coeficiente de rugosidade
        float reflection = glm::pow(specular_dot, rec.rug);

        // Parte especular da iluminação de Phong
        vec3 specular_factor = cur_light->getIntensity() * rec.kespc * reflection;

        // Adiciona os fatores difuso e especular ao somatório
        sum += diffuse_factor + specular_factor;
    }

    // Resultado final, combinando parte ambiental, difusa e especular
    vec3 result = ambient_factor + sum;
    result = clamp(result, 0.0f, 1.0f);  // Clamp para garantir que o resultado esteja dentro do intervalo [0, 1]

    return result;
}

glm::vec3 snells_refract(const glm::vec3& incident, const glm::vec3& normal, float eta) {
    // Pegando ângulo entre vetor de incidência(-1) e normal
    float cos_i = glm::dot(-incident, normal);

    // sen² + cos² = 1 -> senI² = (1 - cosI²)
    // senT = eta * senI
    // senT² = eta² * senI²
    float sin2_t = eta * eta * (1.0f - cos_i * cos_i);
    
    // Caso aconteça, refração total ocorreria (luz "presa"), sendo desconsiderado
    if (sin2_t > 1.0f) {
        return glm::vec3(0.0f);
    }

    // Extraindo cos_T
    float cos_t = sqrt(1.0f - sin2_t);

    // Vetor direção final, escalando, aplicando índice de refração com eta e tendendo a normal
    return eta * incident + (eta * cos_i - cos_t) * normal;
}


// Função para calcular a cor de um raio, dependendo se ele atinge algum objeto no mundo ou não
color ray_color(const ray& r, hitable *world, vec3 cam_position, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001f, FLT_MAX, rec)) {
        vec3 phong_color = phong(rec, ambientLight->getAmbientLight(), scene_lights, cam_position);

        if (depth < 2) {
            // Reflexão
            vec3 V = normalize(r.direction());
            vec3 N = rec.normal;
            vec3 reflected_dir = V - 2.0f * glm::dot(V, N) * N; //
            ray reflected_ray(rec.p, reflected_dir);
            phong_color += rec.kref * ray_color(reflected_ray, world, cam_position, depth + 1);

            // Refração (Lei de Snells)
            if (rec.ktrans > 0.0f) {
                vec3 refracted_dir;
                float eta = rec.ktrans; // Índice de refração
                float cos_i = glm::dot(-V, N);

                if (cos_i > 0) {
                    // Raio entrando no objeto
                    refracted_dir = snells_refract(V, N, eta);
                } else {
                    // Raio saindo do objeto, inverter o índice de refração
                    refracted_dir = snells_refract(V, -N, 1.0f / eta);
                }

                ray refracted_ray(rec.p, refracted_dir);
                phong_color += rec.ktrans * ray_color(refracted_ray, world, cam_position, depth + 1);
            }
        }

        return phong_color;
    }

    color backgroundColor = glm::vec3(0.0,0.0,0.0); // Cor preta para o background
    return backgroundColor;  // Retorna a cor de fundo se o raio não atingir nenhum objeto
}

class BSPNode {
public:
    BSPNode* front;
    BSPNode* back;
    glm::vec3 split_plane_normal;
    float split_plane_d;
    std::vector<triangle*> triangles;

    BSPNode(std::vector<triangle*>& tris, int depth = 0, int max_depth = 10, int min_tris_per_node = 2) 
        : front(nullptr), back(nullptr) 
    {
        if (tris.size() <= min_tris_per_node || depth >= max_depth) {
            triangles = tris; // Leaf node
            return;
        }

        // Plano-cortante (função abaixo)
        selectSplittingPlane(tris);

        // Criando partes back e front do node
        std::vector<triangle*> front_tris;
        std::vector<triangle*> back_tris;

        for (auto* tri : tris) {
            float d1 = glm::dot(split_plane_normal, tri->a) - split_plane_d;
            float d2 = glm::dot(split_plane_normal, tri->b) - split_plane_d;
            float d3 = glm::dot(split_plane_normal, tri->c) - split_plane_d;
            const float epsilon = 1e-6f;

            if (d1 >= -epsilon && d2 >= -epsilon && d3 >= -epsilon) {
                front_tris.push_back(tri);
            } else if (d1 < epsilon && d2 < epsilon && d3 < epsilon) {
                back_tris.push_back(tri);
            } else {
                splitTriangle(tri, front_tris, back_tris);
            }
        }

        if (!front_tris.empty()) {
            front = new BSPNode(front_tris, depth + 1, max_depth, min_tris_per_node);
        }
        if (!back_tris.empty()) {
            back = new BSPNode(back_tris, depth + 1, max_depth, min_tris_per_node);
        }
    }

    ~BSPNode() {
        delete front;
        delete back;
    }

private:
    // Para escolher quais triângulos cortar triângulos quando necessário
    void selectSplittingPlane(const std::vector<triangle*>& tris) {
        // Prioridade para o primeiro triângulo (arbitrário)
        int best_index = 0;
        float best_score = std::numeric_limits<float>::max();

        for (int i = 0; i < tris.size(); ++i) {
            const auto& tri = tris[i];
            glm::vec3 normal = tri->normal;
            float d = glm::dot(normal, tri->a);

            float score = evaluateSplittingPlane(tris, normal, d);
            if (score < best_score) {
                best_score = score;
                best_index = i;
            }
        }

        const auto& best_tri = tris[best_index];
        split_plane_normal = best_tri->normal;
        split_plane_d = glm::dot(split_plane_normal, best_tri->a);
    }

    // Função recomendada para fazer com que o balanceamento da árvore seja facilitado
    float evaluateSplittingPlane(const std::vector<triangle*>& tris, const glm::vec3& normal, float d) {
        int front_count = 0;
        int back_count = 0;
        for (const auto& tri : tris) {
            float d1 = glm::dot(normal, tri->a) - d;
            float d2 = glm::dot(normal, tri->b) - d;
            float d3 = glm::dot(normal, tri->c) - d;

            if (d1 >= 0 || d2 >= 0 || d3 >= 0) front_count++;
            if (d1 <= 0 || d2 <= 0 || d3 <= 0) back_count++;
        }

        return std::abs(front_count - back_count);
    }

    // Fazendo a partição do triângulo e repassando os resultantes para front_tris e back_tris
    void splitTriangle(triangle* tri, std::vector<triangle*>& front_tris, std::vector<triangle*>& back_tris) {
        // Calculate distances from each vertex to the splitting plane
        float d1 = glm::dot(split_plane_normal, tri->a) - split_plane_d;
        float d2 = glm::dot(split_plane_normal, tri->b) - split_plane_d;
        float d3 = glm::dot(split_plane_normal, tri->c) - split_plane_d;

       // definindo os vetores que vão guardar os triângulos resultantes
        std::vector<glm::vec3> front_verts, back_verts;

        // Função auxliar para interpolar entre os dois pontos
        auto interpolate = [](const glm::vec3& p1, const glm::vec3& p2, float d1, float d2) -> glm::vec3 {
            return p1 + (p2 - p1) * (d1 / (d1 - d2));
        };

        // Distribuindo os vértices
        if (d1 >= 0) front_verts.push_back(tri->a); else back_verts.push_back(tri->a);
        if (d2 >= 0) front_verts.push_back(tri->b); else back_verts.push_back(tri->b);
        if (d3 >= 0) front_verts.push_back(tri->c); else back_verts.push_back(tri->c);

        // Caso só um dos vetores armazenantes tenha recebido vértices, não segue com a partição (desnecessário)
        if (front_verts.size() == 3) {
            front_tris.push_back(tri);
            return;
        } else if (back_verts.size() == 3) {
            back_tris.push_back(tri);
            return;
        }

        // Caso tenham vertices em ambos (2 no front pq inclui o próprio original), aí faz a partição
        if (front_verts.size() == 2 && back_verts.size() == 1) {
            // Interpolando para achar ponto de interseção
            glm::vec3 p1 = interpolate(front_verts[0], back_verts[0], glm::dot(split_plane_normal, front_verts[0]) - split_plane_d, glm::dot(split_plane_normal, back_verts[0]) - split_plane_d);
            glm::vec3 p2 = interpolate(front_verts[1], back_verts[0], glm::dot(split_plane_normal, front_verts[1]) - split_plane_d, glm::dot(split_plane_normal, back_verts[0]) - split_plane_d);

            // Criando triângulos Front
            front_tris.push_back(new triangle(front_verts[0], front_verts[1], p1, tri->cor, tri->objMaterial));
            front_tris.push_back(new triangle(front_verts[1], p1, p2, tri->cor, tri->objMaterial));

            // Criando triângulos Back
            back_tris.push_back(new triangle(back_verts[0], p1, p2, tri->cor, tri->objMaterial));
        } else if (front_verts.size() == 1 && back_verts.size() == 2) {
            // Interpolando para achar ponto de interseção
            glm::vec3 p1 = interpolate(back_verts[0], front_verts[0], glm::dot(split_plane_normal, back_verts[0]) - split_plane_d, glm::dot(split_plane_normal, front_verts[0]) - split_plane_d);
            glm::vec3 p2 = interpolate(back_verts[1], front_verts[0], glm::dot(split_plane_normal, back_verts[1]) - split_plane_d, glm::dot(split_plane_normal, front_verts[0]) - split_plane_d);

            // Criando triângulos Front
            front_tris.push_back(new triangle(front_verts[0], p1, p2, tri->cor, tri->objMaterial));

            // Criando triângulos Back
            back_tris.push_back(new triangle(back_verts[0], back_verts[1], p1, tri->cor, tri->objMaterial));
            back_tris.push_back(new triangle(back_verts[1], p1, p2, tri->cor, tri->objMaterial));
        }
    }
};


void logBSPNode(std::ofstream& file, BSPNode* node, int depth = 0) {
    if (node == nullptr) return;

    // Identação baseada no depth
    std::string indent = std::string(depth * 2, ' ');

    if (node->triangles.empty()) {
        // Nodes disponíveis a serem analisados para bipartição
        file << indent << "Internal Node [Depth " << depth << "]:\n";
        file << indent << "  Splitting Plane:\n";
        file << indent << "    Normal = (" 
             << node->split_plane_normal.x << ", " 
             << node->split_plane_normal.y << ", " 
             << node->split_plane_normal.z << ")\n";
        file << indent << "    d = " << node->split_plane_d << "\n";

        // Recurse into child nodes
        file << indent << "  Front Node:\n";
        logBSPNode(file, node->front, depth + 1);
        file << indent << "  Back Node:\n";
        logBSPNode(file, node->back, depth + 1);
    } else {
        // Nodes finais
        file << indent << "Leaf Node [Depth " << depth << "]:\n";
        file << indent << "  Triangles (" << node->triangles.size() << "):\n";

        // Printando vértices dos triângulos
        for (auto* tri : node->triangles) {
            file << indent << "    Triangle: ";
            file << "a(" << tri->a.x << ", " << tri->a.y << ", " << tri->a.z << "), ";
            file << "b(" << tri->b.x << ", " << tri->b.y << ", " << tri->b.z << "), ";
            file << "c(" << tri->c.x << ", " << tri->c.y << ", " << tri->c.z << ")\n";
        }
    }
}

int main() {
    int nx = 500;  // Largura da imagem
    int ny = 500;  // Altura da imagem

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";  // Imprime o cabeçalho do arquivo PPM

    glm::vec3 origin(0.0f, 0.0f, 0.0f);  // Origem da câmera
    glm::vec3 lookingat(0.0f, 0.0f, -1.0f);  // Ponto para onde a câmera está olhando
    glm::vec3 vup(0.0f, 1.0f, 0.0f);  // Vetor de "up" da câmera
    float distance = 0.3f;  // Distância entre a câmera e o plano da imagem


    // Definir alguns triângulos para a cena
    std::vector<triangle*> triangles;

    triangles.push_back(
        new triangle(
            glm::vec3(-1, 0, -1),
            glm::vec3(1, 0, -1),
            glm::vec3(0, 1, -1),
            red,
            mattePlane
        )
    );
    triangles.push_back(
        // new triangle(
        //     glm::vec3(-0.8, 0, -0.8),
        //     glm::vec3(-0.6, 0, -1.3),
        //     glm::vec3(-0.6, 1, -1.0),
        //     green,
        //     mattePlane
        // )  
        new triangle(
            glm::vec3(0, -0.2, -1.5),
            glm::vec3(2, -0.2, -1.0),
            glm::vec3(1, 0.8, -1.5),
            green,
            mattePlane
        )
    );
    triangles.push_back(
      new triangle(
            glm::vec3(-0.5, 0, -0.8),
            glm::vec3(-0.3, 0, -1.3),
            glm::vec3(-0.3, 1, -1.0),
            blue,
            mattePlane
        )  
    );

    // Construir a árvore BSP a partir dos triângulos
    BSPNode* bsp_tree = new BSPNode(triangles);

    logBSPNode(bspLogFile, bsp_tree);

    bspLogFile.close();


    // Cria uma lista de objetos hitable, incluindo duas esferas, dois planos e duas malhas

    int sizeList = 7;

    hitable* list[sizeList];

    //
    Transform transform;
    transform.setTransformationMatrix( glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0.5f, 0)); // Translação

    glm::vec3 centerRedSphere(5, 1, -6);

    list[0] = new sphere(glm::vec3(-4, -2.0, 2), 1.5, red, matte);
    list[1] = new sphere(glm::vec3(0, -2.0, 2), 1.5, blue, glass);
    list[2] = new sphere(glm::vec3(4, -2.0, 2), 1.5, black, mirror);
 
    // list[0] = new sphere(glm::vec3(-4, 0.0, -1), 1.5, red, matte);
    // list[1] = new sphere(glm::vec3(0, 0.0, -1), 1.5, blue, glass);
    // list[2] = new sphere(glm::vec3(4, 0.0, -1), 1.5, black, mirror);

    list[3] = new plane(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0), slate, glossyPlane);

    list[4] = new triangle(
        glm::vec3(-1, 0, -1),
        glm::vec3(1, 0, -1),
        glm::vec3(0, 1, -1),
        red,
        glass
    );

    list[5] = new triangle(
        // glm::vec3(-0.8, 0, -0.8),
        // glm::vec3(-0.6, 0, -1.3),
        // glm::vec3(-0.6, 1, -1.0),
        // green,
        // mattePlane
        glm::vec3(0, -0.2, -1.5),
        glm::vec3(2, -0.2, -1.0),
        glm::vec3(1, 0.8, -1.5),
        blue,
        glass
    );

    list[6] = new triangle(
        glm::vec3(-0.5, 0, -0.8),
        glm::vec3(-0.3, 0, -1.3),
        glm::vec3(-0.3, 1, -1.0),
        green,
        glass
    );

    // ---- Splitting visuals

    list[7] = new triangle(
        glm::vec3(0, -0.2, -1.5),
        glm::vec3(2, -0.2, -1),
        glm::vec3(1, 0.8, -1.5),
        green,
        matte
    );


    list[8] = new triangle(
        glm::vec3(-0.3, 0, -1.3),
        glm::vec3(-0.42, 0, -1),
        glm::vec3(-0.3, 1, -1),
        blue,
        matte
    );

    list[9] = new triangle(
        glm::vec3(-0.3, 1, -1),
        glm::vec3(-0.42, 0, -1),
        glm::vec3(-0.3, 1, -1),
        red,
        glass
    );
    
    // // Define vertices and triangle indices for the mesh
    // glm::vec3 vertices[] = {
    //     glm::vec3(-1, 0, -3),
    //     glm::vec3(1, 0, -3),
    //     glm::vec3(0, 1, -3),
    //     glm::vec3(-1, 0, -2),
    //     glm::vec3(1, 0, -2),
    //     glm::vec3(0, 1, -2),
    //     glm::vec3(-1, 0, -1),
    //     glm::vec3(1, 0, -1),
    //     glm::vec3(0, 1, -1)
    // };

    // std::tuple<int, int, int> indices[] = {
    //     std::make_tuple(0, 1, 2),
    //     std::make_tuple(3, 4, 5),
    //     std::make_tuple(6, 7, 8)
    // };

    // color red(1.0f, 0.0f, 0.0f);
    // color green(0.0f, 1.0f, 0.0f);
    // color blue(0.0f, 0.0f, 1.0f);
    // material matte; // Placeholder material

    // // Create the triangle mesh
    // tmesh* mesh = new tmesh(9, 3, vertices, indices, red, &matte);

    // list[4] = mesh;
    
    // Cria o mundo com a lista de objetos
    hitable* world = new hitable_list(list, sizeList);

    scene_lights.push_back(light_point1);
    scene_lights.push_back(light_point2);
    scene_lights.push_back(light_point3);


    
    camera cam(origin, lookingat, vup, ny, nx, distance);  // Cria uma câmera

    // Loop para gerar a imagem linha por linha
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);  // Coordenada u do pixel normalizada
            float v = float(j) / float(ny);  // Coordenada v do pixel normalizada
            ray r = cam.get_ray(u, v);  // Obtém o raio correspondente ao pixel na câmera
            color pixel_color = ray_color(r, world, cam.get_origin(), 0); // Calcula a cor do raio
            write_color(std::cout, pixel_color);  // Escreve a cor no arquivo PPM
        }
    }

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso
}
