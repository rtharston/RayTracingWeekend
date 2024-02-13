#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// constexpr color red{1.0, 0.0, 0.0};
constexpr color white{1.0, 1.0, 1.0};
constexpr color sky_blue{0.5, 0.7, 1.0};

double hit_sphere(const point3& center, double radius, const ray& r) {
  // See section 5.1 for explanation of this math
  // In short, this checks if the ray hits the sphere by checking if there is
  // a point on the ray that satisfies the formula for the surface of a sphere.
  // The original formula is x^2+y^2+z^2=r^2, but it has been rearranged below.
  const vec3 oc = r.origin() - center;
  const double a = dot(r.direction(), r.direction());
  const double b = 2.0 * dot(oc, r.direction());
  const double c = dot(oc, oc) - radius*radius;
  const double discriminant = b*b - 4*a*c;
  if (discriminant < 0) {
    return -1;
  } else {
    return (-b - sqrt(discriminant)) / (2.0 * a);
  }
}

color ray_color(const ray& r) {
  constexpr point3 centerSphere(0,0,-1);

  const double t = hit_sphere(centerSphere, 0.5, r);
  if (t > 0.0) {
    const vec3 N = unit_vector(r.at(t) - centerSphere);
    return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
  }

  const vec3 unit_direction = unit_vector(r.direction());
  const double a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * white + a * sky_blue;
}

int main() {

  // Image

  constexpr double aspect_ratio = 16.0 / 9.0;
  constexpr int image_width = 400;

  // Calculate the image height, and ensure that it's at least 1.
  constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
  static_assert(image_height > 1, "image height is less than 1");

  // Camera

  constexpr double focal_length = 1.0;
  constexpr double viewport_height = 2.0;
  constexpr double viewport_width = viewport_height * aspect_ratio;
  constexpr point3 camera_center{0, 0, 0};

  // Calculate the vectors across the horizontal and down the vertical viewport edges.
  constexpr vec3 viewport_u{viewport_width, 0, 0};
  constexpr vec3 viewport_v{0, -viewport_height, 0};

  // Calculate the horizontal and vertical delta vectors from pixel to pixel.
  constexpr vec3 pixel_delta_u = viewport_u / image_width;
  constexpr vec3 pixel_delta_v = viewport_v / image_height;

  // Calculate the location of the upper left pixel.
  constexpr point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
  constexpr point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // Render

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; ++j) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      const point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
      // TODO: maybe I should make a ray constructor that takes a point3 as the destination, and calculates the direction.
      const vec3 ray_direction = pixel_center - camera_center;
      const ray r{camera_center, ray_direction};
      
      const color pixel_color = ray_color(r);

      write_color(std::cout, pixel_color);
    }
  }

  // the spaces to cover the scanlines remaining message
  std::clog << "\rDone.                 \n";
  return 0;
}