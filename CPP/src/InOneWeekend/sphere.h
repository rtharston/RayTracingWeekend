#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
  sphere(const point3 _center, const double _radius, const std::shared_ptr<material> _mat) noexcept
  : center{_center},
    radius{_radius},
    mat{_mat}
  {}

  bool hit(const ray& r, const interval ray_t, hit_record& rec) const noexcept override {
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
    if (!ray_t.surrounds(root)) {
      root = (-half_b + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
  }

// private:
  const point3 center;
  const double radius;
  const std::shared_ptr<material> mat;
};

// naive multi-hit method just to test the concept in the code that calls this.
// DO NOT USE `std::bitset<4>` instead of std::array<bool, 4> it slowed my test from 25 to 32 seconds!
std::array<bool, 4> hit_spheres_avx2(const std::array<sphere*, 4> spheres, const ray& r, const interval ray_t, std::array<hit_record, 4>& recs, const int obj_count) noexcept {
  std::array<bool, 4> results;
  
  // The tail of the outer loop might not pass in 4 objects, so ignore the last values
  for (int i = 0; i < obj_count && i < spheres.size(); ++i) {
    const auto& sphere = spheres[i];

    // See sections 5.1 and 6.2 for explanation of this math
    // In short, this checks if the ray hits the sphere by checking if there is
    // a point on the ray that satisfies the formula for the surface of a sphere.
    // The original formula is x^2+y^2+z^2=r^2, but it has been rearranged below.
    const vec3 oc = r.origin() - sphere->center;
    const double a = r.direction().length_squared();
    const double half_b = dot(oc, r.direction());
    const double c = oc.length_squared() - sphere->radius*sphere->radius;

    const double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
      results[i] = false;
      continue; 
    }

    const double sqrtd = sqrt(discriminant);
    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (-half_b + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
      results[i] = false;
      continue; 
      }
    }

    recs[i].t = root;
    recs[i].p = r.at(recs[i].t);
    vec3 outward_normal = (recs[i].p - sphere->center) / sphere->radius;
    recs[i].set_face_normal(r, outward_normal);
    recs[i].mat = sphere->mat;

    results[i] = true;
  }

  return results;
}

#endif // SPHERE_H