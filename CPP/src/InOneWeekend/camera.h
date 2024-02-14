#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"

#include <iosfwd>

class camera {
  public:
    constexpr camera(const double _aspect_ratio, const int _image_width, const int _samples_per_pixel, const int _max_depth) noexcept 
    : aspect_ratio(_aspect_ratio),
      image_width(_image_width),
      image_height(static_cast<int>(image_width / aspect_ratio)),
      samples_per_pixel(_samples_per_pixel),
      max_depth(_max_depth),
      center(0, 0, 0)
    {
      // Ensure that image height is at least 1.
      // static_assert(image_height > 1, "image height is less than 1");

      const double focal_length = 1.0;
      const double viewport_height = 2.0;
      const double viewport_width = viewport_height * aspect_ratio;

      // Calculate the vectors across the horizontal and down the vertical viewport edges.
      const vec3 viewport_u{viewport_width, 0, 0};
      const vec3 viewport_v{0, -viewport_height, 0};

      // Calculate the horizontal and vertical delta vectors from pixel to pixel.
      pixel_delta_u = viewport_u / image_width;
      pixel_delta_v = viewport_v / image_height;

      // Calculate the location of the upper left pixel.
      const point3 viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
      pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    void render(const hittable& world) const noexcept {
      std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

      for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
          color pixel_color(0,0,0);
          for (int sample = 0; sample < samples_per_pixel; ++sample) {
            const ray r = get_ray(i, j);
            pixel_color += ray_color(r, max_depth, world);
          }

          // Dividing by samples_per_pixel gives a slightly different result
          const double scale = 1.0 / samples_per_pixel;
          pixel_color *= scale;

          static const interval intensity(0.000, 0.999);
          pixel_color.clamp(intensity);
          
          write_color(std::cout, pixel_color);
        }
      }

      // the spaces to cover the scanlines remaining message
      std::clog << "\rDone.                 \n";
    }

  private:
    
  color ray_color(const ray& r, const int depth, const hittable& world) const noexcept {
    // If we have exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
      return color(0,0,0);
    
    hit_record record;
    // look for hits that aren't super close to the surface (which are likely due to floating point rounding errors)
    if (world.hit(r, interval(0.001, infinity), record)) {
      // TODO: make a way to easily select which diffusion algorithm I want to use for each object
      // // Simple diffused material with rays bounding in random directions
      // const vec3 direction = random_on_hemisphere(record.normal);

      // Diffused material using Lambertian distribution (darker shadows, more sky color)
      const vec3 direction = record.normal + random_unit_vector();

      // Get the color of the ray that bounced from this hit point
      // Return half the color intensity (to get a gray color)
      return 0.5 * ray_color(ray(record.p, direction), depth-1, world);
    }

    const vec3 unit_direction = unit_vector(r.direction());
    const double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * white + a * sky_blue;
  }

  ray get_ray(const int i, const int j) const noexcept {
    // Get a randomly sampled camera ray for the pixel at location i,j.

    const vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    const vec3 pixel_sample = pixel_center + pixel_sample_square();

    const point3 ray_origin = center;
    const vec3 ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
  }

  vec3 pixel_sample_square() const noexcept {
    // Returns a random point in the square surrounding a pixel at the origin.
    const double px = -0.5 + random_double();
    const double py = -0.5 + random_double();
    return (px * pixel_delta_u) + (py * pixel_delta_v);
  }

  const double aspect_ratio;
  const int image_width;
  const int image_height;
  const int samples_per_pixel;
  const int max_depth;   // Maximum number of ray bounces into scene
  const point3 center;   // Camera center
  point3 pixel00_loc;    // Location of pixel 0, 0
  vec3   pixel_delta_u;  // Offset to pixel to the right
  vec3   pixel_delta_v;  // Offset to pixel below
};

#endif