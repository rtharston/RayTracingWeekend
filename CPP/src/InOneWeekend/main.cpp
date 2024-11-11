#include <iostream>
#include <fstream>

#include "SDL.h"

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

uint8_t* frame_buffer;
uint8_t bpp;
uint32_t pitch;


int main(int argc, char* argv[]) {
  // I tried to use a std::ostream* to choose between std::cout and file, but only cout worked for some reason
  std::ofstream fout;
  if (argc > 2) {
    // I create the file here to fail on errors before wasting time rendering an image I can't save
    fout = std::ofstream{argv[1]};
    if (!fout) {
      return -1;
    }
  }

  int samples_per_pixel = 10;
  if (argc > 1) {
    samples_per_pixel = atoi(argv[1]);
  }

  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  const camera cam(16.0 / 10.0, 2560, samples_per_pixel, 50, 20, point3(13,2,3), point3(0,0,0), vec3(0,1,0), 0.6, 10);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"RayTrace test",
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		cam.image_width, 
		cam.image_height,
		0
	);

	SDL_Surface *surface = SDL_GetWindowSurface(window);
	// srand(1234);

  // TODO: copy frame_buffer to SDL buffer

  // for ()
  // for ()
  // {
  //   frame_buffer


	// 	Uint32 color = rint << 16 | gint << 8 | bint;

	// 	Uint32 *buffer = (Uint32*)((Uint8 *)surface->pixels + y * surface->pitch + x * bpp);
	// 	*buffer = color;
  // }

  // TODO: get actual values from SDL
  bpp = 4;
  pitch = cam.image_width * bpp;
  // frame_buffer = (uint8_t*)malloc(bpp * cam.image_width * cam.image_height);
  frame_buffer = (Uint8 *)surface->pixels;

  // Render a whole line per thread to get better utilization out of each thread.
  auto render_world = [&cam, &world]() {
    cam.render(world);
  };

  const auto render_thread = std::thread(render_world);

	SDL_Event event;
  bool quit = false;
  while (!quit) {
      while (SDL_PollEvent(&event) != 0) {
          switch (event.type) {
              case SDL_QUIT:
              {
                quit = true;
                break;
              }
              default:
                break;
          }
      }

	    SDL_UpdateWindowSurface(window);

      // about 60 fps
      SDL_Delay(17);
  }

	SDL_DestroyWindow(window);
	SDL_Quit();

  // TODO: do this properly in the background as soon as the render is done, and show some sort of output to indicate progress
  // print_to_ppm(argc == 1 ? std::cout : fout, cam.image_width, cam.image_height);
  // save to file if a file name is given
  if (argc > 2) {
    // render_thread.join();
    print_to_ppm(fout, cam.image_width, cam.image_height);
  }

  return 0;
}