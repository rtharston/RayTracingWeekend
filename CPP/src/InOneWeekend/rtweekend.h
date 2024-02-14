#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <random>

// Constants

constexpr double infinity = std::numeric_limits<double>::infinity();

// Utility Functions

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