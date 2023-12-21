#include "vec3.h"

#include <iostream>

int main() {

  // Image

  int image_width = 256;
  int image_height = 256;

  // Render

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; ++j) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      double r = double(i) / (image_width-1);
      double g = double(j) / (image_height-1);
      double b = 0;

      int ir = static_cast<int>(255.99 * r);
      int ig = static_cast<int>(255.99 * g);
      int ib = static_cast<int>(255.99 * b);

      std::cout << ir << ' ' << ig << ' ' << ib << std::endl;
    }
  }

  // the spaces to cover the scanlines remaining message
  std::clog << "\rDone.                 \n";
  return 0;
}