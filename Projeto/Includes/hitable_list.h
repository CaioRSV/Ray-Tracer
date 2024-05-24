#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "./hitable.h"
#include <vector>

class hitable_list: public hitable {
    public:
        hitable_list() {}
        hitable_list(std::vector<hitable*> l, int n) {listv = l; list_size = n;}
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        std::vector<hitable*> listv;
        int list_size;
};


#endif