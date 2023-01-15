pub struct Pixel {
    pub r: u8,
    pub g: u8,
    pub b: u8,
    // a: u8
}

impl Pixel {
    // fn new(r: u8, g: u8, b: u8, a: u8) -> Self {
    //     Pixel { r, g, b, a }
    // }

    pub fn new(r: u8, g: u8, b: u8) -> Self {
        // Pixel { r, g, b, a: 255 }
        Pixel { r, g, b }
    }
}

pub struct RTImage {
    pub pixels: Vec<Pixel>,
    pub width: usize,
    pub height: usize,
}
