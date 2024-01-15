#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
  public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    // TODO: consider making these return const references
    point3 origin() const noexcept { return orig; }
    vec3 direction() const noexcept { return dir; }
    
    point3 at(double t) const noexcept {
      return orig + t * dir;
    }

  private:
    point3 orig;
    vec3 dir;
};

#endif // RAY_H