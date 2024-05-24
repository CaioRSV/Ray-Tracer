#include "./tmesh.h"

// construtor do tmesh
tmesh::tmesh(int n_vertices, int n_triangulos, vec3 vertices[], triple vertices_index[], color cor): n_vertices(n_vertices), n_triangulos(n_triangulos),cor(cor) {
    // cria os triangulos com base nos parametros passados e insere no vector de triangulos
    for (int i = 0; i< n_triangulos; i++){
        vec3 A = vertices[std::get<0>(vertices_index[i])];
        vec3 B = vertices[std::get<1>(vertices_index[i])];
        vec3 C = vertices[std::get<2>(vertices_index[i])];
        triangulos.push_back(triangle(A, B, C, cor));
    }
};

bool tmesh::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{   
    bool mesh_hit = false;
    double closest_so_far = t_max;
    // chama o metodo hit de cada triangulo dentro da mesh
    for (triangle t: triangulos){
        if(t.hit(r, t_min, closest_so_far, rec)){
            mesh_hit = true;
            closest_so_far = rec.t;
        }
    }
    return mesh_hit;
}