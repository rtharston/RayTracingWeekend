#ifndef INTERVAL_H
#define INTERVAL_H

// ininity is in rtweekend.h, and this header is included there
class interval {
public:
  double min, max;

  constexpr interval() noexcept : min(+infinity), max(-infinity) {}

  constexpr interval(const double _min, const double _max) noexcept : min(_min), max(_max) {}

  bool contains(const double x) const noexcept {
    return min <= x && x <= max;
  }

  bool surrounds(const double x) const noexcept {
    return min < x && x < max;
  }

  double clamp(const double x) const noexcept {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  }

  static const interval empty, universe;
};

const static interval empty;
const static interval universe(-infinity, +infinity);

#endif // INTERVAL_H