#include "./triangle.h"

// construtor do triangle
triangle::triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color):a(a), b(b), c(c), cor(color) {
            normal = normalize(cross((b-a), (c-a)));
            // vetor da aresta entre a e b
            glm::vec3 u = b - a;

            // vetor da aresta entre a e c
            glm::vec3 v = c - a;

            // projecao do vetor u em v
            glm::vec3 projuv = dot(u,v)/dot(v,v) * v;

            // projecao do vetor v em u
            glm::vec3 projvu = dot(v,u)/dot(u,u) * u;

            // altura do ponto B
            hb = u - projuv;

            // normalização inversa da altura hb
            hb = hb/dot(hb,hb);

            // altura do ponto C
            hc = v - projvu;

            // normalização inversa da altura hc
            hc = hc/dot(hc,hc);
        };

bool triangle::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    float t = glm::dot((a - r.location()), normal) / glm::dot(r.direction(), normal);

    if (t < t_max && t > t_min)
    {
        // Ponto de interseção com o plano:
        glm::vec3 p = r.point_at_parameter(t);
        
        // Vetor do vertice do triangulo até o ponto p
        glm::vec3 w = p - a;
        
        float beta = glm::dot(w, hb);
        float gamma = glm::dot(w, hc);
        float alpha = 1 - beta - gamma;

        if (alpha >= 0 && beta >= 0 && gamma >= 0)
        {
            rec.t = t;
            rec.p = p;
            rec.normal = normal;
            rec.cor = cor;
            return true;
        }
        return false;
    }
    return false;
}