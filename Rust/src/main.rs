mod image;
use image::{Image, Pixel};
mod output;
mod vec3;
use vec3::Color;

fn hello_world_pixel_gen(width: usize, height: usize, w: usize, h: usize) -> Pixel {
    let r = (w as f64) / (width - 1) as f64;
    let g = (h as f64) / (height - 1) as f64;
    let b = 0.25;

    let color = Color::new(r, g, b);
    Pixel::from_color(color)
}

enum GeneratorOption {
    HellowWorld,
}

impl GeneratorOption {
    fn pixel_generator(self) -> &'static dyn Fn(usize, usize, usize, usize) -> Pixel {
        match self {
            Self::HellowWorld => &hello_world_pixel_gen,
        }
    }
}

fn main() {
    // TODO: get output parameters from arguments
    let width = 256;
    let height = 256;
    let option = GeneratorOption::HellowWorld;

    Image::new(width, height, option.pixel_generator()).print_ppm();
}
