#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
  constexpr sphere(const point3 _center, const double _radius) noexcept : center{_center}, radius{_radius} {}

  bool hit(const ray& r, const double ray_tmin, const double ray_tmax, hit_record& rec) const noexcept override {
    // See sections 5.1 and 6.2 for explanation of this math
    // In short, this checks if the ray hits the sphere by checking if there is
    // a point on the ray that satisfies the formula for the surface of a sphere.
    // The original formula is x^2+y^2+z^2=r^2, but it has been rearranged below.
    const vec3 oc = r.origin() - center;
    const double a = r.direction().length_squared();
    const double half_b = dot(oc, r.direction());
    const double c = oc.length_squared() - radius*radius;

    const double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;

    const double sqrtd = sqrt(discriminant);
    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
      root = (-half_b + sqrtd) / a;
      if (root <= ray_tmin || ray_tmax <= root) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    return true;
  }

private:
  const point3 center;
  const double radius;
};

#endif // SPHERE_H