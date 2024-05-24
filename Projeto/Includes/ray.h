#ifndef RAY_H
#define RAY_H

#include "../../External/glm/glm.hpp"

class ray
{
public:
    glm::vec3 A; // localização de onde sairá o raio 
    glm::vec3 B; // direção que o raio seguirá
    ray();
    ray(const glm::vec3 &a, const glm::vec3 &b); // construtor
    glm::vec3 location() const; // retorna o vetor com a informação localização da camera
    glm::vec3 direction() const; // retorna o vetor de onde a camera está direcionada
    glm::vec3 point_at_parameter(float t) const;
};

#endif