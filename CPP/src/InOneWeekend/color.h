#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

constexpr color red{1.0, 0.0, 0.0};
constexpr color white{1.0, 1.0, 1.0};
constexpr color sky_blue{0.5, 0.7, 1.0};

// Convert from linear to gamma space to correct darkness level
inline double linear_to_gamma(const double linear_component) noexcept {
  return std::sqrt(linear_component);
}

inline void linear_to_gamma(color& c) noexcept {
  c.e[0] = linear_to_gamma(c.e[0]);
  c.e[1] = linear_to_gamma(c.e[1]);
  c.e[2] = linear_to_gamma(c.e[2]);
}

void write_color(std::ostream &out, color pixel_color) {
  // Translate the components to [0,255] and write them out
  out << static_cast<int>(255.99 * pixel_color.x()) << ' '
      << static_cast<int>(255.99 * pixel_color.y()) << ' '
      << static_cast<int>(255.99 * pixel_color.z()) << std::endl;
}

#endif // COLOR_H