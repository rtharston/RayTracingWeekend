mod image;
use image::{Image, Pixel};
mod output;

fn hello_world_pixel_gen(width: usize, height: usize, w: usize, h: usize) -> Pixel {
    let fr = (w as f64) / (width - 1) as f64;
    let fg = (h as f64) / (height - 1) as f64;
    let fb = 0.25;

    let r = (255.999 * fr) as u8;
    let g = (255.999 * fg) as u8;
    let b = (255.999 * fb) as u8;

    Pixel::new(r, g, b)
}

fn main() {
    // TODO: get output parameters from arguments
    let width = 256;
    let height = 256;
    let gen = &hello_world_pixel_gen;

    Image::new(width, height, gen).print_ppm();
}
