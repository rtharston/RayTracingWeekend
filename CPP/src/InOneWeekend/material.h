#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

#include "color.h"

class hit_record;

class material {
public:
  virtual ~material() = default;

  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

// Diffused material using Lambertian distribution (darker shadows, more sky color)
class lambertian : public material {
public:
  constexpr lambertian(const color& a) : albedo(a) {}

  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const noexcept override {
    auto scatter_direction = rec.normal + random_unit_vector();

    // catch the case where the scattered ray is near zero to avoid mathematical issues later
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

private:
  const color albedo;
};

// Shiny metal material reflects rays perfectly
class metal : public material {
public:
  constexpr metal(const color& a) : albedo(a) {}

  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const noexcept override {
    const auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return true;
  }

private:
  const color albedo;
};

// Simple diffused material with rays bounding in completely random directions
class random_diffusion : public material {
public:
  constexpr random_diffusion(const color& a) : albedo(a) {}

  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const noexcept override {
    auto scatter_direction = random_on_hemisphere(rec.normal);

    // catch the case where the scattered ray is near zero to avoid mathematical issues later
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

private:
  const color albedo;
};

#endif // MATERIAL_H