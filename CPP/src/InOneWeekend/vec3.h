#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "interval.h"

using std::sqrt;

class vec3 {
  public:
    double e[3];

    constexpr vec3() : e{0,0,0} {}
    constexpr vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    constexpr double x() const noexcept { return e[0]; }
    constexpr double y() const noexcept { return e[1]; }
    constexpr double z() const noexcept { return e[2]; }

    constexpr vec3 operator-() const noexcept { return vec3(-e[0], -e[1], -e[2]); }
    constexpr double operator[](int i) const noexcept { return e[i]; }
    constexpr double& operator[](int i) noexcept { return e[i]; }

    constexpr vec3& operator+=(const vec3 &v) noexcept {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
    }

    constexpr vec3& operator*=(double t) noexcept {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;
      return *this;
    }

    constexpr vec3& operator/=(double t) noexcept {
      return *this *= 1/t;
    }

    constexpr void clamp(const interval t) noexcept {
      t.clamp(e[0]);
      t.clamp(e[1]);
      t.clamp(e[2]);
    }

    constexpr double length() const noexcept {
      return sqrt(length_squared());
    }

    constexpr double length_squared() const noexcept {
      return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    constexpr bool near_zero() const {
      // Return true if the vector is close to zero idn all dimensions
      const auto s = 1e-8;
      return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    static vec3 random() noexcept {
      return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(const double min, const double max) noexcept {
      return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

// point3 is just an alias for vec3, but useful for clarity in the geometry code
using point3 = vec3;

// Vector Utility functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) noexcept {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

constexpr inline vec3 operator+(const vec3 &u, const vec3 &v) noexcept {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

constexpr inline vec3 operator-(const vec3 &u, const vec3 &v) noexcept {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

constexpr inline vec3 operator*(const vec3 &u, const vec3 &v) noexcept {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

constexpr inline vec3 operator*(const double t, const vec3 &v) noexcept {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

constexpr inline vec3 operator*(const vec3 &v, const double t) noexcept {
  return t * v;
}

// The book has (vec3 v, double t)
constexpr inline vec3 operator/(const vec3 &v, const double t) noexcept {
  return (1/t) * v;
}

constexpr inline double dot(const vec3 &u, const vec3 &v) noexcept {
  return u.e[0] * v.e[0]
       + u.e[1] * v.e[1]
       + u.e[2] * v.e[2];
}

constexpr inline vec3 cross(const vec3 &u, const vec3 &v) noexcept {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

constexpr inline vec3 unit_vector(const vec3 &v) noexcept {
  return v / v.length();
}

inline vec3 random_in_unit_disk() {
  // try random vectors until one is found that lies within a unit disk
  while (true) {
    const vec3 p = vec3(random_double(-1,1), random_double(-1,1), 0);
    if (p.length_squared() < 1)
      return p;
  }
}

inline vec3 random_in_unit_sphere() {
  // try random vectors until one is found that lies within a unit sphere
  while (true) {
    const vec3 p = vec3::random(-1,1);
    if (p.length_squared() < 1)
      return p;
  }
}

inline vec3 random_unit_vector() {
  return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
  const vec3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    return on_unit_sphere;
  else
    return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) noexcept {
  // See section 10.4 for an explanation of the math; v + 2b
  return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, const double etai_over_etat) {
  // See section 11.2 for an explanation of the math
  const double cos_theta = fmin(dot(-uv, n), 1.0);
  const vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
  const vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

#endif // VEC3_H