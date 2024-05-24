#include "./plane.h"

bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    float t = glm::dot((plane_point - r.location()), normal) / glm::dot(r.direction(), normal);

    if(t < t_max && t > t_min){
        rec.t = t;
        rec.p = r.point_at_parameter(t);
        rec.normal = normal;
        rec.cor = cor;
        return true;
    }
    return false;
}