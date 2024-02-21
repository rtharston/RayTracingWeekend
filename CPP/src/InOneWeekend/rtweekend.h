#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <random>

// Constants

#if defined(__AVX2__)
#define USE_SIMD_IN_HIT
// #define __simd_pd_t __m256d
constexpr int simd_lane_count = 4;
#elif defined(__ARM_NEON)
#define USE_SIMD_IN_HIT
// #define __simd_pd_t float64x2_t
constexpr int simd_lane_count = 2;
#endif

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions

constexpr inline double degrees_to_radian(const double degrees) {
  return degrees * pi / 180.0;
}

inline double random_double() noexcept {
  // Returns a random real in [0, 1)
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double random_double(const double min, const double max) noexcept {
  // Returns a random real in [min, max)
  return min + (max-min)*random_double();
}

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif // RTWEEKEND_H