#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
  hittable_list world;

  const auto material_ball = make_shared<lambertian>(red);
  const auto material_ground = make_shared<lambertian>(green);

  world.add(make_shared<sphere>(point3(0,0,-1), 0.5, material_ball));
  world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground));

  constexpr camera cam(16.0 / 9.0, 400, 100, 50);
  cam.render(world);

  return 0;
}