#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "color.h"
#include "../../External/glm/glm.hpp"

// Classe do Livro : ShadeRec e GeometricObject pag 53 do livro
struct hit_record {
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    color cor;
};

class hitable { 
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif