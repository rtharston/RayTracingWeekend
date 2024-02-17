#include <iostream>
#include <fstream>

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main(int argc, char* argv[]) {
  // I tried to use a std::ostream* to choose between std::cout and file, but only cout worked for some reason
  std::ofstream fout;
  if (argc == 2) {
    // I create the file here to fail on errors before wasting time rendering an image I can't save
    fout = std::ofstream{argv[1]};
    if (!fout) {
      return -1;
    }
  }

  hittable_list world;

  const auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  const auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  const auto material_left   = make_shared<dielectric>(1.5);
  const auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

  world.add(make_shared<sphere>(point3( 0.0, -100.5, -1), 100, material_ground));
  world.add(make_shared<sphere>(point3( 0.0,      0, -1), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0,      0, -1), 0.5, material_left));
  world.add(make_shared<sphere>(point3( 1.0,      0, -1), 0.5, material_right));

  constexpr camera cam(16.0 / 9.0, 400, 100, 50);
  cam.render(world, argc == 1 ? std::cout : fout);

  return 0;
}