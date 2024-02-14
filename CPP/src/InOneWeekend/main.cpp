#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
  hittable_list world;
  world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
  world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

  constexpr camera cam(16.0 / 9.0, 400, 100);
  cam.render(world);

  return 0;
}