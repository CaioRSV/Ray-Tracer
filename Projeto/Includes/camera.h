#ifndef CAMERA_H
#define CAMERA_H

#include "./ray.h"

// camera

class camera {
    public:
    //camera() {}
    camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vres, float  hres, float dist);

    ray get_ray(float s, float t);

    glm::vec3 origem;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u, v, w;
};

#endif