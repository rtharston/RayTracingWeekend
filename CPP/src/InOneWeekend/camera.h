#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"

#include <iosfwd>

class camera {
  public:
    constexpr camera(const double _aspect_ratio, const int _image_width) noexcept 
    : aspect_ratio(_aspect_ratio),
      image_width(_image_width),
      image_height(static_cast<int>(image_width / aspect_ratio)),
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
          const point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
          // TODO: maybe I should make a ray constructor that takes a point3 as the destination, and calculates the direction.
          const vec3 ray_direction = pixel_center - center;
          const ray r{center, ray_direction};
          
          const color pixel_color = ray_color(r, world);

          write_color(std::cout, pixel_color);
        }
      }

      // the spaces to cover the scanlines remaining message
      std::clog << "\rDone.                 \n";
    }

  private:
    
  color ray_color(const ray& r, const hittable& world) const noexcept {
    hit_record record;
    if (world.hit(r, interval(0, infinity), record)) {
      return 0.5 * (record.normal + color(1,1,1));
    }

    const vec3 unit_direction = unit_vector(r.direction());
    const double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * white + a * sky_blue;
  }

  const double aspect_ratio;
  const int image_width;
  const int image_height;
  const point3 center;   // Camera center
  point3 pixel00_loc;    // Location of pixel 0, 0
  vec3   pixel_delta_u;  // Offset to pixel to the right
  vec3   pixel_delta_v;  // Offset to pixel below
};

#endif