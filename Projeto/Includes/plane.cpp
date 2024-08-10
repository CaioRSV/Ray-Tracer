#include "plane.h"  
#include <cmath>  

// Método para verificar se um raio atinge o plano
bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // Calcula o parâmetro 't' da interseção do raio com o plano
    float t = glm::dot((plane_point - r.location()), normal) / glm::dot(r.direction(), normal);

    // Verifica se a interseção está dentro do intervalo válido
    if (t < t_max && t > t_min) {
        rec.t = t;  // Atualiza o parâmetro 't' do registro de interseção
        rec.p = r.point_at_parameter(t);  // Calcula o ponto de interseção
        rec.normal = normal;  // Define o vetor normal ao ponto de interseção
        rec.cor = cor;  // Define a cor do objeto

        // Aplicando textura caso possua
        if (texture) {
            float u = glm::dot(rec.p - plane_point, glm::vec3(1.0, 0.0, 0.0));
            float v = glm::dot(rec.p - plane_point, glm::vec3(0.0, 0.0, 1.0));

            // glm::vec3 reference_axis = glm::vec3(0.0, 1.0, 0.0); // Seta o plano Y por padrão
            // if (glm::abs(glm::dot(normal, reference_axis)) > 0.999f) {
            //     // Se tiver quase sendo paralelo, troca pro plano X como referencial
            //     reference_axis = glm::vec3(1.0, 0.0, 0.0);
            // }

            // // Usa o referencial pra achar o vetor normal ao "X do plano da textura"
            // glm::vec3 normal_vector_u = glm::normalize(glm::cross(normal, reference_axis));

            // // Usa o "X" encontrado para definir o "Y do plano da textura"
            // glm::vec3 normal_vector_v = glm::normalize(glm::cross(normal, normal_vector_u));

            // // Faz a projeção do vetor da origem do plano até o ponto P
            // // nos 'planos' gerados pelos vetores normais a eles (X/Y do plano)
            // float u = glm::dot(rec.p - plane_point, normal_vector_u);
            // float v = glm::dot(rec.p - plane_point, normal_vector_v);

            rec.cor = texture->get_color(u, v);
        } else {
            rec.cor = cor;
        }


        // informacoes sobre o material do objeto
        rec.kdif = objMaterial->kd;
        rec.kamb = objMaterial->ka;
        rec.kespc = objMaterial->ks;
        rec.rug = objMaterial->n;

        // info dos materiais para a entraga 5, no momento da entrega 4 usaremos os valores 0
        rec.kref = objMaterial->kr;
        rec.ktrans = objMaterial->kt;

        return true;  // Retorna true para indicar que houve uma interseção
    }
    return false;  // Retorna false se não houve interseção
}

