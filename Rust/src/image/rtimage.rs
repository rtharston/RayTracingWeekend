use super::pixel::Pixel;

pub struct RTImage {
    pub pixels: Vec<Pixel>,
    pub width: usize,
    pub height: usize,
}
