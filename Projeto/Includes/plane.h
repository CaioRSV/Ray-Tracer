#ifndef PLANE_H
#define PLANE_H

#include "./hitable.h"

// plano 
class plane: public hitable {
    public:
        plane() {}
        plane(glm::vec3 p, glm::vec3 n, color c) : plane_point(p), normal(normalize(n)), cor(c) {};
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        glm::vec3 plane_point;
        glm::vec3 normal;
        color cor;
};


#endif