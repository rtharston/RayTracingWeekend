#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, color pixel_color) {
  // Translate the components to [0,255] and write them out
  out << static_cast<int>(255.99 * pixel_color.x()) << ' '
      << static_cast<int>(255.99 * pixel_color.y()) << ' '
      << static_cast<int>(255.99 * pixel_color.z()) << std::endl;
}

#endif // COLOR_H