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

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  for (int a = -12; a < 12; a++) {
    for (int b = -12; b < 12; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  // iMac 5k
  // const camera cam(16.0 / 9.0, 5120, 500, 50, 30, point3(13,2,3), point3(0,0,0), vec3(0,1,0), 0.6, 10);

  // 13" M2 MBA
  const camera cam(2940.0 / 1912.0, 2940, 500, 50, 30, point3(13,2,3), point3(0,0,0), vec3(0,1,0), 0.6, 10);
  cam.render(world, argc == 1 ? std::cout : fout);

  return 0;
}