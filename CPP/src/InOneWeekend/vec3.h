#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

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

    constexpr double length() const noexcept {
      return sqrt(length_squared());
    }

    constexpr double length_squared() const noexcept {
      return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
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

#endif // VEC3_H