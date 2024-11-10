#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

extern uint8_t* frame_buffer;

constexpr color red{1.0, 0.0, 0.0};
constexpr color green{0.0, 1.0, 0.0};
constexpr color blue{0.0, 0.0, 1.0};
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

extern uint8_t bpp;
extern uint32_t pitch;

void write_color(const color pixel_color, const int x, const int y) {
  uint8_t* bfr = frame_buffer + y * pitch + x * bpp;

  // Translate the components to [0,255] and write them out
  bfr[0] = static_cast<int>(255.99 * pixel_color.x());
  bfr[1] = static_cast<int>(255.99 * pixel_color.y());
  bfr[2] = static_cast<int>(255.99 * pixel_color.z());
  bfr[3] = 255; // alpha
}


void print_to_ppm(std::ostream &out, const int image_width, const int image_height) {
  out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int y = 0; y < image_height; ++y) {
    for (int x = 0; x < image_width; ++x) {

      uint8_t* bfr = frame_buffer + y * pitch + x * bpp;

      out << static_cast<int>(bfr[0]) << ' '
          << static_cast<int>(bfr[1]) << ' '
          << static_cast<int>(bfr[2]) << std::endl;
    }
  }
}

void write_color(std::ostream &out, color pixel_color) {
  // Translate the components to [0,255] and write them out
  out << static_cast<int>(255.99 * pixel_color.x()) << ' '
      << static_cast<int>(255.99 * pixel_color.y()) << ' '
      << static_cast<int>(255.99 * pixel_color.z()) << std::endl;
}

#endif // COLOR_H