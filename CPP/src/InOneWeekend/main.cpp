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

std::atomic_bool stop_render = false;

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

  int preview_samples_per_pixel = 1;
  int preview_max_depth = 5;

  int samples_per_pixel = 10;
  int max_depth = 50;
  if (argc > 1) {
    samples_per_pixel = atoi(argv[1]);
  }

  hittable_list world;
  world.objects.reserve(22 * 22);

  const auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  const auto glass_material = make_shared<dielectric>(1.5);

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      const auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        if (choose_mat < 0.8) {
          // diffuse
          const auto albedo = color::random() * color::random();
          world.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
        } else if (choose_mat < 0.95) {
          // metal
          const auto albedo = color::random(0.5, 1);
          const auto fuzz = random_double(0, 0.5);
          world.add(make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
        } else {
          // glass
          world.add(make_shared<sphere>(center, 0.2, glass_material));
        }
      }
    }
  }

  const auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  const auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  const auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  const camera cam(16.0 / 10.0, 2560, 20, point3(13,2,3), point3(0,0,0), vec3(0,1,0), 0.6, 10);

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
  // const int buffer_size = bpp * cam.image_width * cam.image_height;

  // Render first pass preview
  const auto render_preview = [&cam, &world, preview_samples_per_pixel, preview_max_depth]() {
    cam.render(world, preview_samples_per_pixel, preview_max_depth);
  };

  const auto preview_thread = std::thread(render_preview);
  // bool preview_running = true;
  
  bool render_running = false;
  const auto render_world = [&cam, &world, &render_running, samples_per_pixel, max_depth]() {
    render_running = true;
    cam.render(world, samples_per_pixel, max_depth);
    render_running = false;
  };

  std::thread render_thread;

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
              case SDL_KEYUP:
                // TODO: add a pause/continue so I can't accidentally stop and have to start all the way over again
                // TODO: add ability to run another batch of X samples to add to the existing samples, to continue to improve quality
                if (event.key.keysym.sym == SDLK_s) {
                  if (render_running) {
                    stop_render = true;
                    render_thread.join();
                    stop_render = false;
                  } else {
                    // TODO: add ability to change parameters before starting the full render (and do a quick refresh each time)
                    render_thread = std::thread(render_world);
                  }
                }
                // if (clear_screen)
                //   // clear the screen before starting new render
                //   std::memset(frame_buffer, 0, buffer_size);
                // }
                break;
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