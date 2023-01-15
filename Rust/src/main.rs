mod image;
use image::{Pixel, RTImage};
mod output;

fn image_hello_world() -> RTImage {
    let image_width = 256;
    let image_height = 256;
    let mut pixels = Vec::<Pixel>::with_capacity(image_width * image_height);

    for j in (0..image_height).rev() {
        for i in 0..image_width {
            let fr = (i as f64) / (image_width - 1) as f64;
            let fg = (j as f64) / (image_height - 1) as f64;
            let fb = 0.25;

            let r = (255.999 * fr) as u8;
            let g = (255.999 * fg) as u8;
            let b = (255.999 * fb) as u8;

            pixels.push(Pixel::new(r, g, b))
        }
    }

    RTImage {
        pixels: pixels,
        width: image_width,
        height: image_height,
    }
}

fn main() {
    let image = image_hello_world();
    image.print_ppm();
}
