#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
  hittable_list world;

  const auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  const auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
  const auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8));
  const auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));

  world.add(make_shared<sphere>(point3( 0.0, -100.5, -1), 100, material_ground));
  world.add(make_shared<sphere>(point3( 0.0,      0, -1), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0,      0, -1), 0.5, material_left));
  world.add(make_shared<sphere>(point3( 1.0,      0, -1), 0.5, material_right));

  constexpr camera cam(16.0 / 9.0, 400, 100, 50);
  cam.render(world);

  return 0;
}