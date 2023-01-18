pub mod pixel;

pub use pixel::Pixel;

pub struct Image {
    pixels: Vec<Pixel>,
    width: usize,
    height: usize,
}

impl Image {
    pub fn new(
        width: usize,
        height: usize,
        pixel_generator: &dyn Fn(usize, usize, usize, usize) -> Pixel,
    ) -> Image {
        let mut pixels = Vec::<Pixel>::with_capacity(width * height);

        let mut progress_counter = 0;
        for h in (0..height).rev() {
            if progress_counter % 10 == 0 && h != 0 {
                eprintln!("Scanlines remaining: {}", h);
            }
            progress_counter += 1;

            for w in 0..width {
                pixels.push(pixel_generator(width, height, w, h))
            }
        }
        eprintln!("Render complete.");

        Image {
            pixels,
            width,
            height,
        }
    }

    pub fn get_pixels(&self) -> &Vec<Pixel> {
        &self.pixels
    }

    pub fn get_width(&self) -> usize {
        self.width
    }

    pub fn get_height(&self) -> usize {
        self.height
    }
}
