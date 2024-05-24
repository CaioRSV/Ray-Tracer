#ifndef SPHERES_H
#define SPHERES_H

#include "./hitable.h"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(glm::vec3 cen, float r, color c) : center(cen), radius(r), cor(c) {};
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        glm::vec3 center;
        float radius;
        color cor;
};

#endif