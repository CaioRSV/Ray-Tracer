#ifndef COLOR_H
#define COLOR_H

#include "../../External/glm/glm.hpp"

#include <iostream>

using color = glm::vec3;

void write_color(std::ostream &out, color pixel_color);

#endif