#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "interval.h"
#include "rtweekend.h"

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

#if defined(__AVX2__)
#include <immintrin.h>

// Passing in separate point3s is faster than passing an array of point3
inline void vec3_sub1_avx(const vec3 &u, const point3 &v_0, const point3 &v_1, const point3 &v_2, const point3 &v_3, __m256d& x, __m256d& y, __m256d& z) {
  __m256d m_u_x = _mm256_set1_pd(u.e[0]);
  __m256d m_u_y = _mm256_set1_pd(u.e[1]);
  __m256d m_u_z = _mm256_set1_pd(u.e[2]);

  // letting the compiler do this gives me a faster result then using _mm256_setr_pd; maybe it is using gather methods for me?
  __m256d m_v_x = {v_0.e[0], v_1.e[0], v_2.e[0], v_3.e[0]};
  __m256d m_v_y = {v_0.e[1], v_1.e[1], v_2.e[1], v_3.e[1]};
  __m256d m_v_z = {v_0.e[2], v_1.e[2], v_2.e[2], v_3.e[2]};

  x = m_u_x - m_v_x;
  y = m_u_y - m_v_y;
  z = m_u_z - m_v_z;

  // This is slower.
  // __m256d x = _mm256_sub_pd(m_u_x, m_v_x);
  // __m256d y = _mm256_sub_pd(m_u_y, m_v_y);
  // __m256d z = _mm256_sub_pd(m_u_z, m_v_z);
  
  // results[0] = vec3(x[0], y[0], z[0]);
  // results[1] = vec3(x[1], y[1], z[1]);
  // results[2] = vec3(x[2], y[2], z[2]);
  // results[3] = vec3(x[3], y[3], z[3]);
}

inline __m256d vec3_dot1_avx(const vec3 &u, const __m256d& x, const __m256d& y, const __m256d& z) {
  __m256d m_u_x = _mm256_set1_pd(u.e[0]);
  __m256d m_u_y = _mm256_set1_pd(u.e[1]);
  __m256d m_u_z = _mm256_set1_pd(u.e[2]);

  return m_u_x * x
         + m_u_y * y
         + m_u_z * z;
}

inline __m256d vec3_length_squared_avx(const __m256d& x, const __m256d& y, const __m256d& z) {
  return x*x + y*y + z*z;
}

// inline std::array<vec3,4> vec3_sub_avx(const std::array<vec3,4> &u, const std::array<vec3,4> &v) {
//   return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
// }
#elif defined(__ARM_NEON)
#include <arm_neon.h>

// Passing in separate point3s is faster than passing an array of point3
inline void vec3_sub1_neon(const vec3 &u, const point3 &v_0, const point3 &v_1, float64x2_t& x, float64x2_t& y, float64x2_t& z) {
  float64x2_t m_u_x = _mm256_set1_pd(u.e[0]);
  float64x2_t m_u_y = _mm256_set1_pd(u.e[1]);
  float64x2_t m_u_z = _mm256_set1_pd(u.e[2]);

  // letting the compiler do this gives me a faster result then using _mm256_setr_pd; maybe it is using gather methods for me?
  float64x2_t m_v_x = {v_0.e[0], v_1.e[0]};
  float64x2_t m_v_y = {v_0.e[1], v_1.e[1]};
  float64x2_t m_v_z = {v_0.e[2], v_1.e[2]};

  x = m_u_x - m_v_x;
  y = m_u_y - m_v_y;
  z = m_u_z - m_v_z;
}

inline float64x2_t vec3_dot1_neon(const vec3 &u, const float64x2_t& x, const float64x2_t& y, const float64x2_t& z) {
  float64x2_t m_u_x = _mm256_set1_pd(u.e[0]);
  float64x2_t m_u_y = _mm256_set1_pd(u.e[1]);
  float64x2_t m_u_z = _mm256_set1_pd(u.e[2]);

  return m_u_x * x
         + m_u_y * y
         + m_u_z * z;
}

inline float64x2_t vec3_length_squared_neon(const float64x2_t& x, const float64x2_t& y, const float64x2_t& z) {
  return x*x + y*y + z*z;
}

// inline std::array<vec3,4> vec3_sub_neon(const std::array<vec3,4> &u, const std::array<vec3,4> &v) {
//   return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
// }
#endif

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