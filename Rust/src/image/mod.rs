pub mod pixel;

pub use pixel::Pixel;

pub struct Image {
    pixels: Vec<Pixel>,
    width: usize,
    height: usize,
}

impl Image {
    pub fn new(pixels: Vec<Pixel>, width: usize, height: usize) -> Image {
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
