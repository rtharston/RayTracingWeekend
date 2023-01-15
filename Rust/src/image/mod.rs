pub mod pixel;

pub use pixel::Pixel;

pub struct Image {
    pub pixels: Vec<Pixel>,
    pub width: usize,
    pub height: usize,
}
