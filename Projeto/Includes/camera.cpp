#include "./camera.h"

camera::camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vres, float  hres, float dist)
{
    w = glm::normalize(lookfrom - lookat);
    u = glm::normalize(glm::cross(vup,w));
    v = glm::cross(w,u);
    
    origem = lookfrom;
    float proporcao = vres / hres;
    
    // tamanho da largura e altura
    float xsize = 1.0f;
    float ysize = xsize * proporcao;
    
    // metades das proporcoes
    float mLargura = xsize / 2;
    float mAltura = mLargura * proporcao; 
    
    float cameraDistance = dist;

    //lower_left_corner = glm::vec3(-mLargura, -mAltura, -cameraDistance);
    lower_left_corner = origem - (mLargura*u) -(mAltura*v) - w;

    horizontal = 2*mLargura*u;
    vertical = 2*mAltura*v;
    
}

ray camera::get_ray(float s, float t)
{
    return ray(origem, lower_left_corner + s*horizontal + t*vertical - origem);
}
