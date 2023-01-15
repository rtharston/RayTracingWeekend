mod image;
use image::{Pixel, RTImage};
mod ppm;

fn image_hello_world() -> RTImage {
    // let image_width = 256;
    // let image_height = 256;
    // let mut pixels = Vec::<Pixel>::new();

    // RTImage { pixels: pixels, width: image_width, height: image_height }

    let pixels = vec![
        Pixel::new(255, 0, 0),
        Pixel::new(0, 255, 0),
        Pixel::new(0, 0, 255),
        Pixel::new(255, 0, 0),
    ];
    RTImage {
        pixels: pixels,
        width: 2,
        height: 2,
    }
}

fn main() {
    let image = image_hello_world();
    image.print_ppm();
}
