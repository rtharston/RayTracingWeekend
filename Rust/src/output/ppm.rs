impl crate::Pixel {
    fn ppm_string(&self) -> String {
        format!("{} {} {}", self.r, self.g, self.b)
    }
}

impl crate::Image {
    pub fn ppm_string(self) -> String {
        let ppm_header = format!("P3\n{} {}\n255\n", self.get_width(), self.get_height());
        let ppm_string = self
            .get_pixels()
            .iter()
            .map(|x| x.ppm_string() + "\n")
            .collect::<String>();
        format!("{}{}", ppm_header, ppm_string)
    }
}
