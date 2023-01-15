impl crate::Pixel {
    fn ppm_string(&self) -> String {
        format!("{} {} {}", self.r, self.g, self.b)
    }
}

impl crate::Image {
    pub fn print_ppm(self) {
        println!("P3");
        println!("{} {}", self.get_width(), self.get_height());
        println!("255");

        for pixel in self.get_pixels() {
            println!("{}", pixel.ppm_string())
        }
    }
}
