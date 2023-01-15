mod image;
use image::{Image, Pixel};
mod output;

fn image_hello_world() -> Image {
    let image_width = 256;
    let image_height = 256;
    let mut pixels = Vec::<Pixel>::with_capacity(image_width * image_height);

    let mut progress_counter = 0;
    for j in (0..image_height).rev() {
        if progress_counter % 10 == 0 && j != 0 {
            eprintln!("Scanlines remaining: {}", j);
        }
        progress_counter += 1;

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
    eprintln!("Done.");

    Image {
        pixels: pixels,
        width: image_width,
        height: image_height,
    }
}

fn main() {
    let image = image_hello_world();
    image.print_ppm();
}
