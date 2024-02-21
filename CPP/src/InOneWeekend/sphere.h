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
std::array<bool, 4> hit_spheres_avx2(const std::shared_ptr<sphere> spheres[], const ray& r, const double a, const interval ray_t, std::array<hit_record, 4>& recs, const int sphere_count) noexcept {
  std::array<bool, 4> results;
  
  if (sphere_count < 4) {
    for (int i = 0; i < sphere_count; ++i) {
      results[i] = spheres[i]->hit(r, ray_t, recs[i]);
    }

    return results;
  }

  // See sections 5.1 and 6.2 for explanation of this math
  // In short, this checks if the ray hits the sphere by checking if there is
  // a point on the ray that satisfies the formula for the surface of a sphere.
  // The original formula is x^2+y^2+z^2=r^2, but it has been rearranged below.

  // TODO: convert the parts with doubles into AVX

  const vec3 oc = r.origin() - spheres[0]->center;
  const double half_b = dot(oc, r.direction());
  const double c = oc.length_squared() - spheres[0]->radius * spheres[0]->radius;

  const double discriminant = half_b*half_b - a*c;
  if (discriminant < 0)
    results[0] = false;
  else {
    results[0] = true;
    const double sqrtd = sqrt(discriminant);
    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (-half_b + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
      results[0] = false;
      }
    }

    if (results[0]) {
      recs[0].t = root;
      recs[0].p = r.at(recs[0].t);
      vec3 outward_normal = (recs[0].p - spheres[0]->center) / spheres[0]->radius;
      recs[0].set_face_normal(r, outward_normal);
      recs[0].mat = spheres[0]->mat;
    }
  }

  {
    const vec3 oc = r.origin() - spheres[1]->center;
    const double half_b = dot(oc, r.direction());
    const double c = oc.length_squared() - spheres[1]->radius * spheres[1]->radius;

    const double discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
      results[1] = false;
    else {
      results[1] = true;
      const double sqrtd = sqrt(discriminant);
      // Find the nearest root that lies in the acceptable range.
      double root = (-half_b - sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        root = (-half_b + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
        results[1] = false;
        }
      }

      if (results[1]) {
        recs[1].t = root;
        recs[1].p = r.at(recs[1].t);
        vec3 outward_normal = (recs[1].p - spheres[1]->center) / spheres[1]->radius;
        recs[1].set_face_normal(r, outward_normal);
        recs[1].mat = spheres[1]->mat;
      }
    }
  }

  {
    const vec3 oc = r.origin() - spheres[2]->center;
    const double half_b = dot(oc, r.direction());
    const double c = oc.length_squared() - spheres[2]->radius * spheres[2]->radius;

    const double discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
      results[2] = false;
    else {
      results[2] = true;
      const double sqrtd = sqrt(discriminant);
      // Find the nearest root that lies in the acceptable range.
      double root = (-half_b - sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        root = (-half_b + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
        results[2] = false;
        }
      }

      if (results[2]) {
        recs[2].t = root;
        recs[2].p = r.at(recs[2].t);
        vec3 outward_normal = (recs[2].p - spheres[2]->center) / spheres[2]->radius;
        recs[2].set_face_normal(r, outward_normal);
        recs[2].mat = spheres[2]->mat;
      }
    }
  }

  {
    const vec3 oc = r.origin() - spheres[3]->center;
    const double half_b = dot(oc, r.direction());
    const double c = oc.length_squared() - spheres[3]->radius * spheres[3]->radius;

    const double discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
      results[3] = false;
    else {
      results[3] = true;
      const double sqrtd = sqrt(discriminant);
      // Find the nearest root that lies in the acceptable range.
      double root = (-half_b - sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        root = (-half_b + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
        results[3] = false;
        }
      }

      if (results[3]) {
        recs[3].t = root;
        recs[3].p = r.at(recs[3].t);
        vec3 outward_normal = (recs[3].p - spheres[3]->center) / spheres[3]->radius;
        recs[3].set_face_normal(r, outward_normal);
        recs[3].mat = spheres[3]->mat;
      }
    }
  }

  return results;
}

#endif // SPHERE_H